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

function pebbleSend(message) {
  const info = `Message sent from PKJS ${JSON.stringify(message)}`;
  console.log(`Sending message from PKJS ${JSON.stringify(message)}`);
  Pebble.sendAppMessage(
    message,
    function () {
      console.log("Succes " + info);
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
        VALUE: tasks.length,
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
