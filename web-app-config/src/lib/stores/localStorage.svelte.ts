/* 
  This localStorage code was written by Rich Harris himself so it must be good
  https://github.com/Rich-Harris/local-storage-test/blob/main/src/lib/storage.svelte.ts

  Example: 
  
  ```js
  import { LocalStorage } from '$lib/stores/adapters/localStorage.svelte';

	const box = new LocalStorage('box', {
		color: '#ff3e00',
		dimensions: [100, 100]
	});
	
	```
*/

import { tick } from 'svelte';

export class LocalStorage<T> {
	#key: string;
	#version = $state(0);
	#listeners = 0;
	#value: T | undefined;

	#handler = (e: StorageEvent) => {
		if (e.storageArea !== localStorage) return;
		if (e.key !== this.#key) return;

		this.#version += 1;
	};

	constructor(key: string, empty: T, initial?: T) {
		this.#key = key;
		this.#value = empty;

		if (typeof localStorage !== 'undefined') {
			if (localStorage.getItem(key) === null) {
				localStorage.setItem(key, JSON.stringify(initial));
			}
		}
	}

	get value() {
		this.#version;

		const root =
			typeof localStorage !== 'undefined'
				? JSON.parse(localStorage.getItem(this.#key) as any)
				: this.#value;

		const proxies = new WeakMap();

		const proxy = (value: unknown) => {
			if (typeof value !== 'object' || value === null) {
				return value;
			}

			let p = proxies.get(value);

			if (!p) {
				p = new Proxy(value, {
					get: (target, property) => {
						this.#version;
						return proxy(Reflect.get(target, property));
					},
					set: (target, property, value) => {
						this.#version += 1;
						Reflect.set(target, property, value);

						if (typeof localStorage !== 'undefined') {
							localStorage.setItem(this.#key, JSON.stringify(root));
						}

						return true;
					}
				});

				proxies.set(value, p);
			}

			return p;
		};

		if ($effect.tracking()) {
			$effect(() => {
				if (this.#listeners === 0) {
					window.addEventListener('storage', this.#handler);
				}

				this.#listeners += 1;

				return () => {
					tick().then(() => {
						this.#listeners -= 1;
						if (this.#listeners === 0) {
							window.removeEventListener('storage', this.#handler);
						}
					});
				};
			});
		}

		return proxy(root);
	}

	set value(value) {
		if (typeof localStorage !== 'undefined') {
			localStorage.setItem(this.#key, JSON.stringify(value));
		}

		this.#version += 1;
	}
}
