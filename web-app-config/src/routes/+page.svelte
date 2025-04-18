<script lang="ts">
    import { LocalStorage } from '$lib/stores/localStorage.svelte';
	type Tasks = string;
	type Routine = {
		// Future attributes can go into this object
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
	const ROUTINE_LIMIT = 10;
	const ROUTINE_CHAR_LIMIT = 10;
	const TASK_CHAR_LIMIT = 80;

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

	let oldRoutineName = $state<string>('');
	let editingRoutineName = $state<string>('');
	let editingRoutine = $state<Routine>({
		// startHour: null,
		// startMinute: null,
		tasks: []
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
		console.log('Final data being sent to pebble kit js');
		console.log(JSON.stringify(settings.value.routines));
		document.location = return_to + encodeURIComponent(JSON.stringify(settings.value.routines));
	}

	const routineExamples = ['Morning', 'Brew Tea', 'Clean Room', 'Evening', 'Cook food'];
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

	let newRoutineModal: HTMLDialogElement;
	let newRoutineFromTemplateModal: HTMLDialogElement;
	function clearEditingRoutine() {
		editingRoutineName = '';
		// editingRoutine.startHour = null;
		// editingRoutine.startMinute = null;
		editingRoutine.tasks = [];
		newRoutineModal.close();
		newRoutineFromTemplateModal.close();
	}

	const templateRoutines = {
		Morning: [
			'Feed Cat',
			'Drink Water',
			'Brush Teeth',
			'Duo Lingo',
			'Shower',
			'Make bed',
			'Measure weight',
			'Eat breakfast'
		],
		Evening: [
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
		Poutine: [
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
		Terces: [
			'Congrats!',
			"You've found the secret message!",
			'Thanks for trying out my app!',
			'I hope you like it!',
			'And if you have suggestions, send them my way :D',
			'Email: binamkayastha@gmail.com',
			'You can also join us on the Rebble Discord server!',
			'You can find us on the MeowTime Thread in projects section.',
			'Happy Routining!'
		]
	};

	let templateSelect;
	function addRoutine(name: keyof typeof templateRoutines) {
		userSettings.value.routines[name] = { tasks: templateRoutines[name] };
		templateSelect.selectedIndex = 0;
		newRoutineFromTemplateModal.close();
	}
	
	const routinesLength= $derived(Object.keys(userSettings.value.routines).length)
</script>

<div class="flex h-svh flex-col gap-2 overflow-scroll p-10 text-center">
	<h1 class="pb-2 text-xl">MeowTime Settings</h1>
	<h2 class="text-md pb-1">Your Routines</h2>
	{#each Object.entries(userSettings.value.routines || {}) as [routineName, routine] (routineName)}
		<div class="flex items-baseline gap-2 rounded-md border-2 p-1 shadow-md">
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
					oldRoutineName = routineName;
					editingRoutineName = routineName;
					editingRoutine = routine;
					newRoutineModal.showModal();
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
	<button
		class="btn btn-primary"
		onclick={() =>
			routinesLength >= ROUTINE_LIMIT
				? alert("Sorry you can't create more than 10 routines")
				: newRoutineModal.showModal()}>Create Routine</button
	>
	<button
		class="btn btn-primary"
		onclick={() =>
			routinesLength >= ROUTINE_LIMIT
				? alert("Sorry you can't create more than 10 routines")
				: newRoutineFromTemplateModal.showModal()}>Create Routine From Template</button
	>
	<div class="flex-1"></div>
	<button class="btn btn-secondary" onclick={sendSettingsToPebble}>Submit to Pebble</button>
	<p>Make sure to restart the app to see changes!</p>

	<!-- TODO: Move dialogs to their own components -->
	<dialog bind:this={newRoutineModal} id="new_routine_modal" class="modal">
		<div class="modal-box">
			<h3 class="pb-5 text-lg font-bold">Add a new routine!</h3>
			<div class="flex flex-col gap-2">
				<h1 class="pt-5 text-lg">Routine Name:</h1>
				<div class="flex w-full flex-col">
					<input
						type="text"
						placeholder={`Routine Name ex. ${getRandom(routineExamples)}`}
						class="input invalid:validator"
						required
						pattern={`.{1,${ROUTINE_CHAR_LIMIT}}`}
						bind:value={editingRoutineName}
					/>
					<div class="validator-hint hidden">Routine name must be between 1 and {ROUTINE_CHAR_LIMIT} characters</div>
				</div>
				<h1 class="pt-5 text-lg">Add Tasks to Routine</h1>
				{#each editingRoutine.tasks as task, index}
					<div class="flex gap-2">
						<div class="flex w-full flex-col">
							<input
								type="text "
								placeholder={`Task Name ex. ${getRandom(taskExamples)}`}
								class="input invalid:validator"
								bind:value={editingRoutine.tasks[index]}
								required
								pattern={`.{1,${TASK_CHAR_LIMIT}}`}
								title=""
							/>
							<div class="validator-hint hidden">Task name must be between 1 and {TASK_CHAR_LIMIT} characters</div>
						</div>
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
							errors.push('You forgot to add the routine name. Routines need names bruv.');
						}
						if (editingRoutineName.length > ROUTINE_CHAR_LIMIT) {
							errors.push("Your routine name is too long, it won't fit on the screen.");
						}
						if (editingRoutine.tasks?.length === 0) {
							errors.push(
								"Routine needs at least one task! Don't tell me you're procrastinating on your routines already!"
							);
						}
						if (editingRoutine.tasks?.some((task) => !task)) {
							errors.push(
								'At least one of your routine tasks is empty. Doing nothing is not a task - unfortunately.'
							);
						}
						if (editingRoutine.tasks?.some((task) => task.length > TASK_CHAR_LIMIT)) {
							errors.push(
								'At least one of your routine tasks is too long.'
							);
						}
						if (errors.length > 0) {
							alert(errors.join('\nOne more thing. '));
							return;
						}
						userSettings.value.routines[oldRoutineName] = undefined;
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

	<dialog bind:this={newRoutineFromTemplateModal} id="new_routine_modal" class="modal">
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
					{#each Object.keys(templateRoutines) as name}
						<option>{name}</option>
					{/each}
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
