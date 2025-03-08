# MeowTime Config Page

This is the page used for configuring the pebble app. When the user clicks the settings cog for MeowTime in the pebble app, this page is what loads.

The page is hosted on Cloudflare Pages and that's where it's served from.

The only file of importance at the moment is src/routes/+page.svelte 

In retrospect it may have been overkill to use SvelteKit for this, but I've
built so many SvelteKit static pages that it's my "golden path" for whenever
I want to create a static page, and it enables me to expand the settings page
in the future.

## Creating a project

If you're seeing this, you've probably already done this step. Congrats!

```bash
# create a new project in the current directory
npx sv create

# create a new project in my-app
npx sv create my-app
```

## Developing

Once you've created a project and installed dependencies with `npm install` (or `pnpm install` or `yarn`), start a development server:

```bash
npm run dev

# or start the server and open the app in a new browser tab
npm run dev -- --open
```

## Building

To create a production version of your app:

```bash
npm run build
```

You can preview the production build with `npm run preview`.

> To deploy your app, you may need to install an [adapter](https://svelte.dev/docs/kit/adapters) for your target environment.
