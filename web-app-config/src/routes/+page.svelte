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

	let new_routine_modal: any;
	const routineExamples = [
		'Morning Routine',
		'Brew Coffee',
		'Clean Elephant',
		'Evening Routine',
		'Cook food'
	];
	const taskExamples = [
		'Feed Routini',
		'Do a dance',
		'Open Catnip Drawer',
		'Charge your Pebble',
		"Hydrate yo'self",
		'Brush teeth',
		'Clean litterbox',
		'Run a marathon'
	];
	function getRandom<T>(arr: Array<T>): T {
		return arr[Math.floor(Math.random() * arr.length)];
	}
</script>

<div class="flex h-svh flex-col gap-5 overflow-scroll p-10 text-center">
	<h1 class="text-xl">MeowTime Settings</h1>
	Your Routines
	<button class="btn btn-primary" onclick={() => new_routine_modal.showModal()}
		>Create Routine</button
	>
	<div class="flex-1"></div>
	<button class="btn btn-secondary" onclick={sendSettingsToPebble}>Submit to Pebble</button>

	<dialog bind:this={new_routine_modal} id="new_routine_modal" class="modal">
		<div class="modal-box">
			<h3 class="pb-5 text-lg font-bold">Add a new routine!</h3>
			<div class="flex flex-col gap-2">
				<h1 class="pt-5 text-lg">Routine Name:</h1>
				<input
					type="text"
					placeholder={`Routine Name ex. ${getRandom(routineExamples)}`}
					class="input"
				/>
				<h1 class="pt-5 text-lg">Add Tasks to Routine</h1>
				<input
					type="text "
					placeholder={`Task Name ex. ${getRandom(taskExamples)}`}
					class="input"
				/>
				<button class="btn btn-primary">Add new task</button>
			</div>

			<div class="modal-action flex justify-between">
				<form method="dialog">
					<!-- if there is a button in form, it will close the modal -->
					<button class="btn">Cancel</button>
				</form>
				<form method="dialog">
					<!-- if there is a button in form, it will close the modal -->
					<button class="btn btn-secondary">Add Routine</button>
				</form>
			</div>
			<form method="dialog">
				<button class="btn btn-sm btn-circle btn-ghost absolute top-2 right-2">✕</button>
			</form>
		</div>
		<form method="dialog" class="modal-backdrop">
			<button>close</button>
		</form>
	</dialog>
</div>

<!-- <input type="time" class="input"/> -->

<style>
	input {
		width: 100%;
	}
	.modal-backdrop {
		/* Make modal backdrop darker so users don't think they can click on the elements behind the modal */
		background-color: rgba(0, 0, 0, 0.7);
	}
</style>
