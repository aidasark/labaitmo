$(document).ready(function(){
	$(".hs").css("color","green");
	$(".Search").css("fontSize","25px");
	$(".Search, .Twitch").css("fontWeight","bold");
	$(".Twitch").css("color","purple");
	$("[href]").prepend("↗");
	$("[href]").attr("target","_blank");
	$("[href^='http://']").each(function() {
		$(this).attr("href", $(this).attr("href").replace("http://", "https://"));
	});
	$("button").click(function(){
		$("[href]").each(function() {
			$(this).contents().eq(0).remove();
		});
		$("[href]").removeAttr("target");
		$(this).prop("disabled", true);
	});

	let tbl = document.createElement("table");
	let tblBody = document.createElement("tbody");
	let row = document.createElement("tr");
	let cell = document.createElement("td");
	let cellText = document.createElement("div");
	cell.appendChild(cellText);
	cellText.innerHTML="text";
	let cell2 = document.createElement("td");
	let cellText2 = document.createElement("div");
	cell2.appendChild(cellText2);
	cellText2.innerHTML="FX";
	row.appendChild(cell);
	row.appendChild(cell2);
	tblBody.appendChild(row);
	for (var i = 0; i < 5; i++) {
		let row = document.createElement("tr");
		let cell = document.createElement("td");
		let cellText = document.createElement("div");
		cellText.setAttribute("class", i);
		cellText.innerHTML="someText";
		cell.appendChild(cellText);
		row.appendChild(cell);
		let cell2 = document.createElement("td");
		let cellText2 = document.createElement("button");
		cellText2.setAttribute("class", i);
		cell2.appendChild(cellText2);
		row.appendChild(cell2);
		tblBody.appendChild(row);
	}
	tbl.appendChild(tblBody);
	document.getElementsByTagName('body')[0].appendChild(tbl);
	tbl.setAttribute('border', '2');
	$("button.0").html("fadeOut");
	$("button.0").click(function(){
		$("div.0").fadeOut();
	});
	$("div.1").hide();
	$("button.1").html("fadeIn");
	$("button.1").click(function(){
		$("div.1").fadeIn();
	});
	$("button.2").html("slideUp");
	$("button.2").click(function(){
		$("div.2").slideUp();
	});
	$("div.3").hide();
	$("button.3").html("slideDown");
	$("button.3").click(function(){
		$("div.3").slideDown();
	});
	$("button.4").html("animate");
	$("button.4").click(function(){
		$("div.4").animate({fontSize: '10px'});
	});

	let butt = document.createElement("button");
	butt.setAttribute("id", "ajax");
	butt.innerHTML="Refresh";
	document.getElementsByTagName('body')[0].appendChild(butt);
	$("button#ajax").click(function(){
		$("p.number").load("https://inxaoc.github.io/example/ajax-1.html");
    });

    let butt2 = document.createElement("button");
	butt2.setAttribute("id", "ajax2");
	butt2.innerHTML="Send";
	document.getElementsByTagName('body')[0].appendChild(butt2);

	$("button#ajax2").click(function(){
		let temp = document.createElement("div");
		temp.setAttribute("id", "temp");
		document.getElementsByTagName('body')[0].appendChild(temp);
		$(temp).load("https://inxaoc.github.io/example/ajax.json",
			"authorization", onComplete);
    });
});

$(document).ready(function(){
	$("form *").prop("disabled", true);
});

function onComplete(){
	var json = $(temp).html();
	$(temp).html("");
	var text = "";
	for (var i = 0; i < json.length; i++) {
		if (json[i] == '{' || json[i] == '[') {
			text += "<ul><li>";
		} else if (json[i] == '}' || json[i] == ']') {
			text += "</ul></ul>";
		} else if (json[i] == ',') {
			text += "</li><li>";
		} else if (json[i] == '"') {
			if (json[i-1] == '{') {
				text += "<li>";
			} else if (json[i+1] == '}') {
				text += "</li>";
			}
		} else text +=json[i];

	}
	$(temp).append(text)
}