function content() {
    var info_div = document.getElementById('info');
    var info_content_div = document.getElementById('info_content');
    if (info_div.style.height == "30px") {
        $("#xdiv").fadeOut(1000);
        $("#info_content").fadeIn(1000);
        $("#info").animate({ left: "10%", width: "80%" });
        $("#info").animate({ top: "10%", height: "80%" });
    } else {
        $("#info").animate({ height: "30px", top: "50%" });
        $("#info").animate({ width: "30px", left: "90%" });
        $("#info_content").fadeOut(1000);
        $("#xdiv").fadeIn(1000);
    }
}
