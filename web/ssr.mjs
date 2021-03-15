import puppeteer from 'puppeteer';

var HOST = "http://localhost"
var url_params = '?';

async function ssr(path) {

	// Formatting url for request
	var postalCodeByCity = [];

	postalCodeByCity["COLOMBES"]="92700";
	postalCodeByCity["HOUILLES"]="78800";
	postalCodeByCity["PUTEAUX"]="92800";
	postalCodeByCity["NANTERRE"]="92000";

	function getPostalCode(cityname)
	{
	  return postalCodeByCity[cityname.toUpperCase()];
	}

	function build_params(url){
		var pathname = url.split("/").slice(1);

		var s_city = pathname[3].split("-");

		if(s_city[0].toUpperCase() in postalCodeByCity && getPostalCode(s_city[0]) == s_city[1])
		{
			var city_param = s_city[0][0].toUpperCase();
			city_param += s_city[0].slice(1);
			url_params += "search_city=" + city_param;
			console.log('CITY OK');
		}
	  	else {
	    	throw new Error('Unable to parse CITY');
	  	}
	  	if(pathname[1] == "achat")
	  	{
	    	url_params += "&search_type=1";
	    	console.log('SEARCH TYPE OK');
	  	}
	  	else {
		    if(pathname[1] == "location")
	    	{
	      		url_params += "&search_type=2";
	      		console.log('SEARCH TYPE OK');
	    	}
	    	else {
	      		throw new Error('Unable to parse SEARCH TYPE');
	    	}
	  	}

	  	if(pathname[2] == "appartements")
	  	{
	    	url_params += "&prop_type=1";
	    	console.log('PROP TYPE OK');
	  	}else {
	    	if(pathname[2] == "maisons")
	    	{
	      		url_params += "&prop_type=2";
	      		console.log('PROP TYPE OK');
	    	}
			else if( pathname[2] == "appartements-studios")
	    	{
	      		url_params += "&prop_type=1";
	      		url_params += "&rooms=1";
	      		console.log('PROP TYPE OK');
	    	}else
	    	{
	      		throw new Error('Unable to parse PROP TYPE');
	    	}
	  	}
	  	console.log('ALL IS OK');
		return true;
	}

	function build_params_detail(path)
	{
		var id = path.split('?')[1];
		return HOST + '/annonce_detaille.html?' + id;
	}

	var url = HOST +  path;
	console.log('url before building:' + url);

	if(path.includes("liste-annonces"))
	{
		if (build_params(path))
		{
			url = HOST + '/liste_annonces.html' + url_params;
		}
	}
	else if(path.includes("annonce"))
	{
		url = build_params_detail(path);
	}
	else if(path.includes("actualite-immobilier.html"))
	{
		url = HOST + '/actualite-immo.html';
	}

	console.log('url after building: ' + url);
	
	const start =  Date.now();

	const browser = await puppeteer.launch();
	const page = await browser.newPage();
	
	page
    .on('console', message =>
      console.log(`${message.type().substr(0, 3).toUpperCase()} ${message.text()}`))
    .on('pageerror', ({ message }) => console.log(message))
    .on('response', response =>
      console.log(`${response.status()} ${response.url()}`))
    .on('requestfailed', request =>
      console.log(`${request.failure().errorText} ${request.url()}`));


    await page.setDefaultTimeout(0);
    try {
		await page.goto(url);
		console.log('Waiting for #target');
		await page.waitForSelector('#target');
		console.log('#target loaded');
    } catch(err) {
    	console.error(err);
    	throw new Error('page.goto/waitForselector time out.');
    }

	const html = await page.content();
	await browser.close();

	const ttRenderMs = Date.now() - start;
	console.log(`Headless rendered page in: ${ttRenderMs}ms`);


	return {html, ttRenderMs};
}

export { ssr as default };