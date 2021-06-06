$(function() {
	$('#uploadImage1').on("change", function()
	{
		var files = !!this.files ? this.files : [];
		if (!files.length || !window.FileReader) return;

		if (/^image/.test(files[0].type)){
			var reader = new FileReader();
			reader.readAsDataURL(files[0]);
			
			reader.onloadend = function(){
				$("#imagePreview1").css("background-image", "url("+this.result+")");
				$("#imagePreview1").css("background-color", "");
				$("#icon1").css("display", "none");
				$("#span1").css("display", "none");
				$("#overlay-1").css("display", "flex");
			}
		}
	});

	$('#uploadImage2').on("change", function()
	{
		var files = !!this.files ? this.files : [];
		if (!files.length || !window.FileReader) return;

		if (/^image/.test(files[0].type)){
			var reader = new FileReader();
			reader.readAsDataURL(files[0]);

			reader.onloadend = function(){
				$("#imagePreview2").css("background-image", "url("+this.result+")");
				$("#imagePreview2").css("background-color", "");
				$("#icon2").css("display", "none");
				$("#overlay-2").css("display", "flex");
			}
		}
	});

	$('#uploadImage3').on("change", function()
	{
		var files = !!this.files ? this.files : [];
		if (!files.length || !window.FileReader) return;

		if (/^image/.test(files[0].type)){
			var reader = new FileReader();
			reader.readAsDataURL(files[0]);

			reader.onloadend = function(){
				$("#imagePreview3").css("background-image", "url("+this.result+")");
				$("#imagePreview3").css("background-color", "");
				$("#icon3").css("display", "none");
				$("#overlay-3").css("display", "flex");
			}
		}
	});

	$('#uploadImage4').on("change", function()
	{
		var files = !!this.files ? this.files : [];
		if (!files.length || !window.FileReader) return;

		if (/^image/.test(files[0].type)){
			var reader = new FileReader();
			reader.readAsDataURL(files[0]);

			reader.onloadend = function(){
				$("#imagePreview4").css("background-image", "url("+this.result+")");
				$("#imagePreview4").css("background-color", "");
				$("#icon4").css("display", "none");
				$("#overlay-4").css("display", "flex");
			}
		}
	});

	$('#uploadImage5').on("change", function()
	{
		var files = !!this.files ? this.files : [];
		if (!files.length || !window.FileReader) return;

		if (/^image/.test(files[0].type)){
			var reader = new FileReader();
			reader.readAsDataURL(files[0]);

			reader.onloadend = function(){
				$("#imagePreview5").css("background-image", "url("+this.result+")");
				$("#imagePreview5").css("background-color", "");
				$("#icon5").css("display", "none");
				$("#overlay-5").css("display", "flex");
			}
		}
	});

	// TODO(Stephane Fedim): Control video size, max=20Mb
	// TODO(Stephane Fedim): Insert choice between video and images
	// TODO(Stephane Fedim): Update data sent to the deposit server
	// HACK(Stephane Fedim): Add video compression before upload and decompression on reading
	var video = $("video");
	var thumbnail = $("canvas");
	var input = $("input[type=file]");
	var ctx = thumbnail.get(0).getContext("2d");
	var duration = 0;

	$('#uploadVideo').on("change", function()
	{
		var files = !!this.files ? this.files : [];
		if (!files.length || !window.FileReader) return;
		
		var reader = new FileReader();
		reader.readAsDataURL(files[0]);
		
		file = files[0];
		
		// Set video source
		video.find('source').attr('src', URL.createObjectURL(file));
		
		// Load video
		video.get(0).load();
		
		// Wait for metadata to get duration and dimensions
		video.on('loadedmetadata', function(e) {
			duration = video.get(0).duration;
			// Set canvas dimensions same as video
			thumbnail[0].width = video[0].videoWidth;
			thumbnail[0].height = video[0].videoHeight;
			// Set video current time to get some random image
			video[0].currentTime = Math.ceil(duration/2);
			// Draw base-64 encoded image data when time updates
			video.on('timeupdate', function() {
				ctx.drawImage(video[0], 0, 0, video[0].videoWidth, video[0].videoHeight);
				$("#videoPreview").css("background-image", "url("+thumbnail[0].toDataURL()+")");
				$("#videoPreview").css("background-color", "");
				$("#icon_video").css("display", "none");
				$("#span_video").css("display", "none");
				$("#overlay-video").css("display", "flex");
				sessionStorage.setItem('video', file);
			})
		})
		
		// reader.onloadend = function(){
		// 	$("#videoPreview").css("background-image", "url("+this.result+")");
		// 	$("#videoPreview").css("background-color", "");
		// 	$("#icon_video").css("display", "none");
		// 	$("#overlay-video").css("display", "flex");
		// }
	
	});
});

$("#imagePreview1").click(function() {
	$("#uploadImage1").click();
});

$("#imagePreview2").click(function() {
	$("#uploadImage2").click();
});

$("#imagePreview3").click(function() {
	$("#uploadImage3").click();
});

$("#imagePreview4").click(function() {
	$("#uploadImage4").click();
});

$("#imagePreview5").click(function() {
	$("#uploadImage5").click();
});

$("#videoPreview").click(function() {
	$("#uploadVideo").click();
});