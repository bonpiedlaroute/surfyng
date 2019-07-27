var mobile   = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent);

if(!mobile)
{
  var footerImage1 = document.getElementById("footer_image1");
  var footerImage2 = document.getElementById("footer_image2");

  if( footerImage1 && footerImage2)
  {
    footerImage1.className = "estimate_results_footer_image";
    footerImage2.className = "estimate_results_footer_image_last";
  }
}

const Params = new URLSearchParams(window.location.search);
const predicted_price = Params.get('predicted_price');

var price = document.getElementById("predicted_price");
price.innerHTML = predicted_price;
price.innerHTML += " € <sup>* ";
