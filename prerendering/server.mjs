import express from 'express';
import puppeteer from 'puppeteer'
import ssr from './ssr.mjs';

import { createRequire } from 'module';
const require = createRequire(import.meta.url);

let browserWSEndpoint = null;
const app = express();

app.get('/*', async (request, response, next) => {

   if (!browserWSEndpoint) 
   {
     const browser = await puppeteer.launch({
    headless: true,
    args: ['--single-process', '--no-zygote', '--no-sandbox']
  });
     browserWSEndpoint = await browser.wsEndpoint();
   }

	const {html, ttRenderMs} =  await ssr(request.originalUrl, browserWSEndpoint);

	console.log('Successful ssr, time to render: ' + ttRenderMs);

	// Server-Timing
	response.set('Server-Timing', `Prerender;dur=${ttRenderMs};desc="Headless render time (ms)"`);
	return response.status(200).send(html);
})

var config = require('./config.json');

app.listen(parseInt(config.port, 10), () => console.log('Server started on port: %s. Press Ctrl+C to quit', config.port));
