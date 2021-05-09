
var target = document.getElementById("prerendered-page");
var data = target ? JSON.parse(target.getAttribute("data-ad-json")) : "";

function prevPage()
{
    if (current_page > 1) {
        current_page--;
        changePage(current_page);
        generate_actu(data,true);
        window.scrollTo({ top: 0});
    }
}

function nextPage()
{
    if (current_page < numPages()) {
        current_page++;
        changePage(current_page);
        generate_actu(data,true);
        window.scrollTo({ top: 0});
    }
}

function changePage(page)
{
    var btn_next = document.getElementById("btn_next");
    var btn_prev = document.getElementById("btn_prev");

    var page_span = document.getElementById("list_page");
    var range_page = document.getElementById("range-pagination");

    // Validate page
    if (page < 1) page = 1;
    if (page > numPages()) page = numPages();


    page_span.innerHTML = page + "/" + numPages();
    var min_page = (current_page-1) * records_per_page + 1;
    var max_page = Math.min((current_page * records_per_page),  data.length);
    range_page.innerHTML = min_page + " - " + max_page;

    if (page == 1) {
        btn_prev.style.visibility = "hidden";
    } else {
        btn_prev.style.visibility = "visible";
    }

    if (page == numPages()) {
        btn_next.style.visibility = "hidden";
    } else {
        btn_next.style.visibility = "visible";
    }
}

function numPages()
{
    return Math.ceil(data.length / records_per_page);
}
