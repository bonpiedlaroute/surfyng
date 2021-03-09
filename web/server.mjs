import express from 'express';
import ssr from './ssr.mjs';


const app = express();

app.get('/*', async (request, response, next) => {
	
	const {html, ttRenderMs} =  await ssr(request.originalUrl);
	
	console.log('Successful ssr, time to render: ' + ttRenderMs);
	
	// Server-Timing
	response.set('Server-Timing', `Prerender;dur=${ttRenderMs};desc="Headless render time (ms)"`);
	return response.status(200).send(html);
})

app.listen(8080, () => console.log('Server started on port: 8080. Press Ctrl+C to quit'));
