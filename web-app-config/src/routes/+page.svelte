<script type="ts">
  /* 
    This page is what is loaded when the pebble app user clicks the setting cog
    for MeowTime in the pebble app.
    When the final "submit" button is clicked, the page is redirected to
    pebblejs://closed# (or the emulator return to url) which causes the page to
    close and the information encoded in the URI to be sent to PebbleJS Kit
    (which is found in src/js/index.js).
  */
  function sendSettingsToPebble(settings: Record<string, any>) {
    // Adapted from: https://developer.rebble.io/developer.pebble.com/guides/user-interfaces/app-configuration-static/index.html
    // Determine the correct return URL (emulator vs real watch)
    function getQueryParam(variable: string): string | void {
      // Finds the query param value of the variable input
      const query = location.search.substring(1);
      const vars = query.split('&');
      for (let i = 0; i < vars.length; i++) {
        const pair = vars[i].split('=');
        if (pair[0] === variable) {
          return decodeURIComponent(pair[1]);
        }
      }
    }
    const return_to = getQueryParam('return_to') || 'pebblejs://close#';
    // Encode and send the data when the page closes
    document.location = return_to + encodeURIComponent(JSON.stringify(settings));
  }
</script>

<p>This is an example HTML forms configuration page.</p>
