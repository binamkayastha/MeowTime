<script lang="ts">
	type Tasks = string;
	type Routine = {
		// TODO: Possibly use zod to limit these hour and minute values to 0-23 and 0-59
		// TODO: Possibly support full scope of `RFC 2445` Make as flexible as google calendar rules
		// For now make it daily and based on a specific time of day.
		// These values are passed directly as is to the C code from which the time will be determined
		// as the local time of the pebble.
		// startHour: number;
		// startMinute: number;
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

	// TODO: figure out why there is lag in getting local storage, and show loading
	const userSettings = new LocalStorage<UserSettings>(
		'userSettings',
		{ routines: {} },
		{
			routines: {
				'Example Morning Routine': {
					// startHour: 9,
					// startMinute: 30,
					tasks: ['Wake up', 'Feed Cat', 'Brush Teeth', 'Drink water', 'Jog']
				}
			}
		}
	);

	let editingRoutineName = $state<string>('');
	let editingRoutine = $state<Routine>({
		// startHour: null,
		// startMinute: null,
		tasks: []
	});

	function sendSettingsToPebble() {
		const settings: Record<string, any> = userSettings;
		const simplerDictionary = {};
		debugger;
		for (const [routineName, routine] of Object.entries(settings.value.routines)) {
			console.log('routineName: ' + routineName);
			console.log('routine tasks: ' + routine.tasks.join(','));
			simplerDictionary[routineName] = routine.tasks.join(',');
		}

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
		console.log('Final data being sent to pebble kit js');
		console.log(JSON.stringify(simplerDictionary));
		document.location = return_to + encodeURIComponent(JSON.stringify(simplerDictionary));
	}

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

	let new_routine_modal: HTMLDialogElement;
	let new_routine_from_template_modal: HTMLDialogElement;
	function clearEditingRoutine() {
		editingRoutineName = '';
		// editingRoutine.startHour = null;
		// editingRoutine.startMinute = null;
		editingRoutine.tasks = [];
		new_routine_modal.close();
		new_routine_from_template_modal.close();
	}

	const template_routines = {
		'Morning Routine': [
			'Feed Cat',
			'Drink Water',
			'Brush Teeth',
			'Duo Lingo',
			'Shower',
			'Make bed',
			'Measure weight',
			'Eat breakfast'
		],
		'Evening Routine': [
			'Budgeting',
			'Put away dishes on table',
			'Brush teeth',
			'Fill water bottle for the night',
			'Journal',
			'Create plan for tomorrow',
			'Read Stuff',
			'Pet cat',
			'Put out work cloths',
			'Set alarm for tomorrow morning',
			'Put phone in drawer'
		],
		'Lunchtime Poutine': [
			'Soruce: seasonsandsuppers.ca/authentic-canadian-poutine-recipe/',
			'Gather the following ingredients',
			'3 Tbsp cornstarch',
			'2 Tbsp water',
			'6 Tbsp unsalted butter',
			'1/4 cup unbleached all purpose flour',
			'20 oz beef broth',
			'10 oz chicken broth',
			'Pepper, to taste',
			'Prepare the gravy: In bowl, dissolve cornstarch in water. Set aside.',
			'In saucepan, melt butter.',
			'Add flour and cook, stir regularly, 5 minutes, till it turns golden brown.',
			'Add beef and chicken broth, bring to a boil, stirring with a whisk.',
			'Stir in about HALF the cornstarch mixture and simmer for a minute or so.',
			'Season with pepper. Add salt, if necessary, to taste.',
			'Keep gravy warm till fries are ready',
			'Add baked fries to a large, clean bowl. Season lightly with salt while still warm.',
			'Add a ladle of hot poutine gravy to the bowl and using tongs, toss the fries in the gravy.',
			'Add more gravy, as needed to mostly coat the fries.',
			'Add the cheese curds and toss with the hot fries and gravy.',
			'Serve with freshly ground pepper. Serve immediately. Serve to cat (if safe).'
		],
		'Cat Yoga': [
			'Lay on back',
			'Cat tail pose left',
			'Cat tail pose right',
			'Cat Cow',
			'Extended Cat Pose left',
			'Extended Cat Pose right',
			'Cat Cow',
			'Cobra',
			'Child Pose'
		],
		'Feed Cat': [
			'Feed Cat',
			'Pet Cat',
			'Groom Cat',
			'Open catnip drawer and leave it open',
			'Go to work'
		],
		'Huh?': [
			'What?',
			'Huh?',
			'Oh no',
			'It seem like my cat Routini has taken over the app :o',
			"Don't worry, we'll get this sorted out post hackathon with real templates",
			'Thanks for trying out my app!',
			"I'm very tired, it's 5:03 AM and I've been up all night making cat checklists",
			"I don't even have a cat. I don't even know if they can eat poutine.",
			'Thanks for reading so far!',
			'I hope you like it!',
			'And if you have suggestions, send them my way :D',
			'Email: binamkayastha@gmail.com'
		]
	};

	let templateSelect;
	function addRoutine(name: keyof template_routines) {
		userSettings.value.routines[name] = { tasks: template_routines[name] };
		templateSelect.selectedIndex = 0;
		new_routine_from_template_modal.close();
	}
</script>

<div class="flex h-svh flex-col gap-5 overflow-scroll p-10 text-center">
	<h1 class="text-xl">MeowTime Settings</h1>
	Your Routines
	{#each Object.entries(userSettings.value.routines || {}) as [routineName, routine] (routineName)}
		<div class="flex items-baseline gap-2 rounded-md border-2 bg-gray-700 p-1">
			<div>
				{routineName}
			</div>
			<div class="flex-1"></div>

			<!-- <div class="rounded-md bg-gray-500 p-1">
				{routine.startHour}:{routine.startMinute}
			</div> -->
			<!-- TODO add edit and delete -->
			<button
				class="btn btn-primary"
				onclick={() => {
					editingRoutineName = routineName;
					editingRoutine = routine;
					new_routine_modal.showModal();
				}}>Edit</button
			>
			<button
				class="btn btn-primary"
				onclick={() => {
					const { [routineName]: _, ...newState } = userSettings.value.routines;
					userSettings.value.routines = newState;
				}}>Delete</button
			>
		</div>
	{/each}
	<button class="btn btn-primary" onclick={() => new_routine_modal.showModal()}
		>Create Routine</button
	>
	<button class="btn btn-primary" onclick={() => new_routine_from_template_modal.showModal()}
		>Create Routine From Template</button
	>
	<div class="flex-1"></div>
	<button class="btn btn-secondary" onclick={sendSettingsToPebble}>Submit to Pebble</button>

	<!-- TODO: Move dialogs to their own components -->
	<dialog bind:this={new_routine_modal} id="new_routine_modal" class="modal">
		<div class="modal-box">
			<h3 class="pb-5 text-lg font-bold">Add a new routine!</h3>
			<div class="flex flex-col gap-2">
				<h1 class="pt-5 text-lg">Routine Name:</h1>
				<input
					type="text"
					placeholder={`Routine Name ex. ${getRandom(routineExamples)}`}
					class="input"
					bind:value={editingRoutineName}
				/>
				<h1 class="pt-5 text-lg">Add Tasks to Routine</h1>
				{#each editingRoutine.tasks as task, index}
					<div class="flex gap-2">
						<input
							type="text "
							placeholder={`Task Name ex. ${getRandom(taskExamples)}`}
							class="input"
							bind:value={editingRoutine.tasks[index]}
						/>
						<button
							class="btn btn-primary"
							onclick={() => {
								console.log('before');
								console.log(JSON.stringify(editingRoutine));
								editingRoutine.tasks = editingRoutine.tasks
									.slice(0, index)
									.concat(editingRoutine.tasks.slice(index + 1));
								console.log(JSON.stringify(editingRoutine));
								console.log('after');
							}}>Delete</button
						>
					</div>
				{/each}
				<button class="btn btn-primary" onclick={() => editingRoutine.tasks.push('')}>
					Add new task</button
				>
			</div>

			<div class="modal-action flex justify-between">
				<button class="btn" onclick={clearEditingRoutine}>Cancel</button>
				<button
					class="btn btn-secondary"
					onclick={() => {
						console.log('should add another routine here');
						const errors = [];

						// TODO: like visually update the form for the error at somepoint
						if (!editingRoutineName) {
							errors.push('You forgot to add the routine name. Routines need names bubba.');
						}
						if (editingRoutine.tasks?.length === 0) {
							errors.push(
								"Routine needs at least one task! Don't tell me you're procrastinating on your routines already!"
							);
						}
						if (editingRoutine.tasks?.some((routine) => !routine)) {
							errors.push(
								'One of your routine tasks is empty. Doing nothing is not a task - unfortunately.'
							);
						}
						if (errors.length > 0) {
							alert(errors.join('\nOne more thing. '));
							return;
						}
						userSettings.value.routines[editingRoutineName] = editingRoutine;
						clearEditingRoutine();
					}}>Save Routine</button
				>
			</div>
			<button
				class="btn btn-sm btn-circle btn-ghost absolute top-2 right-2"
				onclick={clearEditingRoutine}>✕</button
			>
		</div>
		<div class="modal-backdrop">
			<button onclick={clearEditingRoutine}>close</button>
		</div>
	</dialog>

	<dialog bind:this={new_routine_from_template_modal} id="new_routine_modal" class="modal">
		<div class="modal-box">
			<h3 class="pb-5 text-lg font-bold">
				Add a new routine from our super duper uber useful templates!
			</h3>
			<div class="flex flex-col gap-2">
				<select
					class="select"
					bind:this={templateSelect}
					onchange={(e) => addRoutine(e.target.value)}
				>
					<option disabled selected>Pick a routine</option>
					<option>Morning Routine</option>
					<option>Evening Routine</option>
					<option>Lunchtime Poutine</option>
					<option>Cat Yoga</option>
					<option>Feed Cat</option>
					<option>Huh?</option>
				</select>
				<form method="dialog">
					<button class="btn btn-sm btn-circle btn-ghost absolute top-2 right-2">✕</button>
				</form>
			</div>
		</div>

		<form method="dialog" class="modal-backdrop">
			<button>close</button>
		</form>
	</dialog>
</div>

<!-- <input type="time" class="input"/> -->

<style>
	input,
	select {
		width: 100%;
	}
	.modal-backdrop {
		/* Make modal backdrop darker so users don't think they can click on the elements behind the modal */
		background-color: rgba(0, 0, 0, 0.7);
	}
</style>
