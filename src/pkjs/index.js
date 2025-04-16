Pebble.addEventListener("showConfiguration", function () {
  // TODO: Move URL to my domain (or another domain)
  // var url = "https://meowtime.binam.dev/";
  var url = "https://meowtime.pages.dev/";

  Pebble.openURL(url);
});

// var transferInProgress = false;

Pebble.addEventListener("ready", function () {
  // PebbleKit JS is ready!
  console.log("PebbleKit JS ready!");
});


const queue = []
let inFlight = 0
function pebbleSend(message) {
  queue.push(message);
  if (inFlight >= 10) {
    // Let the current messages in queue start processing the
    // next message keeping 10 messages in flight
    console.log(`Queued message from PKJS ${JSON.stringify(message)}`);
    return;
  }
  sendNextInQueue(message)
}

function sendNextInQueue() {
  const message = queue.shift();
  const info = `Message sent from PKJS ${JSON.stringify(message)}`;
  console.log(`Sending message from PKJS ${JSON.stringify(message)}`);
  console.log(`Increased inFlight to ${++inFlight}`)
  Pebble.sendAppMessage(
    message,
    function () {
      console.log("Success " + info);
      console.log(`Decreased inFlight to ${--inFlight}`)
      if (queue) {
        sendNextInQueue()
      }
    },
    function (e) {
      console.log("Error " + info);
    }
  );
}

Pebble.addEventListener("webviewclosed", function (e) {
  // Decode the user's preferences
  try{
    var configData = JSON.parse(decodeURIComponent(e.response));
    console.log(`Recieved from config site ${JSON.stringify(configData)}`);
    pebbleSend({ PART: "start" });
    let pebbleKey = 0;
    // First message is the number of routines
    pebbleSend({
      PART: "key_pair",
      KEY: pebbleKey,
      VALUE: Object.keys(configData).length,
    });
    // Then loop through each routine
    for (const [routineName, routineAttrs] of Object.entries(configData)) {
      const tasks = routineAttrs.tasks
      pebbleSend({
        PART: "key_pair",
        KEY: ++pebbleKey,
        VALUE: 1 + tasks.length, // Routine Name + tasks
      });
      pebbleSend({
        PART: "key_pair",
        KEY: ++pebbleKey,
        VALUE: routineName,
      });
      for (const task of tasks) {
        pebbleSend({
          PART: "key_pair",
          KEY: ++pebbleKey,
          VALUE: task,
        });
      }
    }
    pebbleSend({ PART: "end" });
  } catch (error) {
    console.error(error);
  }
});

Pebble.addEventListener("appmessage", function (e) {
  console.log("Got message: " + JSON.stringify(e));
});
