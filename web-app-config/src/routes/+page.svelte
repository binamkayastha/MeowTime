<script lang="ts">
	type Tasks = string;
	type Routine = {
		name: string;
		// TODO: Possibly use zod to limit these hour and minute values to 0-23 and 0-59
		// TODO: Possibly support full scope of `RFC 2445` Make as flexible as google calendar rules
		// For now make it daily and based on a specific time of day.
		// These values are passed directly as is to the C code from which the time will be determined
		// as the local time of the pebble.
		startHour: number;
		startMinute: number;
		tasks: Array<Tasks>;
	};
	type UserSettings = {
		routines: Record<string, Routine>;
	};

	/* 
    This page is what is loaded when the pebble app user clicks the setting cog
    for MeowTime in the pebble app.
    When the final "submit" button is clicked, the page is redirected to
    pebblejs://closed# (or the emulator return to url) which causes the page to
    close and the information encoded in the URI to be sent to PebbleJS Kit
    (which is found in src/js/index.js).
  */

	import { LocalStorage } from '$lib/stores/localStorage.svelte';

	const userSettings = new LocalStorage<UserSettings>('userSettings', {
		routines: {}
	});

	function sendSettingsToPebble() {
		const settings: Record<string, any> = userSettings;

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

	// TODO: install daisy
</script>

<div class="flex h-svh flex-col gap-5 overflow-scroll p-10 text-center">
	<h1 class="text-xl">MeowTime Settings</h1>
	Your Routines
	<button class="btn btn-primary">+ Create Routine</button>
	<div class="flex-1"></div>
	<button class="btn btn-secondary" onclick={sendSettingsToPebble}>Submit to Pebble</button>
</div>

<!-- <input type="time" class="input"/> -->
