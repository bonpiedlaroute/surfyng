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
			}
		}
	});
});

$("#imagePreview1").click(function() {
	$("#uploadImage1").click();
});
$("#icon1").click(function() {
	$("#uploadImage1").click();
});
$("#span1").click(function() {
	$("#uploadImage1").click();
});


$("#imagePreview2").click(function() {
	$("#uploadImage2").click();
});
$("#icon2").click(function() {
	$("#uploadImage2").click();
});


$("#imagePreview3").click(function() {
	$("#uploadImage3").click();
});
$("#icon3").click(function() {
	$("#uploadImage3").click();
});

$("#imagePreview4").click(function() {
	$("#uploadImage4").click();
});
$("#icon4").click(function() {
	$("#uploadImage4").click();
});