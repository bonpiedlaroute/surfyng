function reverse(s){
    return s.split("").reverse().join("");
}

function formatPrice(p)
{
  var val = reverse(p);

  var tmp = val.replace(/(.{3})/g,"$1 ");

  return reverse(tmp);
}
