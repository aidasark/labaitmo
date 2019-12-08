function saveText(row, column){
	let text = document.getElementsByTagName("table")[0].children[0]
		.children[row].children[column].children[0].value;
	document.getElementsByTagName("table")[0].remove();
	document.write(text);
}

function changeWidth(){
	let width = document.getElementsByTagName("div")[0].children[0].value;
	let border = document.getElementsByTagName("div")[0].children[1].value;
	document.getElementsByTagName("table")[0].style = "width: " +  width
		+ "px; border: 2px " + border + ' black;';
}
function changeWidthText(){
	let width = document.getElementsByTagName("div")[0].children[0].value;
	let border = document.getElementsByTagName("div")[0].children[1].value;
	document.getElementsByTagName("div")[0].children[2].value = "Submit with width " + width
		+ "px and " + border + " border";
}

function addHeader(){
	let header = document.getElementsByTagName("div")[1].children[0].value
	document.getElementsByTagName("tbody")[0].innerHTML = "<tr><th colspan='"
		+ document.getElementsByTagName("tbody")[0].children[1].children.length + "'>" + header + "</th></tr>"
		+ document.getElementsByTagName("tbody")[0].innerHTML;
}

function deleteRow(){
	let row = document.getElementsByTagName("div")[2].children[0].value;
	let tableRows = document.getElementsByTagName("tbody")[0].children.length;
	try {
		document.getElementsByTagName("tbody")[0].children[row-1].remove();
	}
	catch {
		alert("400 Bad Request");
	}
}

function magic(){
	let row = Math.floor(Math.random()*document.getElementsByTagName("table")[0]
		.children[0].children.length);
	let column = Math.floor(Math.random()*document.getElementsByTagName("table")[0]
		.children[0].children[row].children.length);
	let cellText = document.getElementsByTagName("table")[0].children[0]
	.children[row].children[column];
	let color1 = Math.floor(Math.random() * 256);
	let color2 = Math.floor(Math.random() * 256);
	let color3 = Math.floor(Math.random() * 256);
	switch (Math.ceil(Math.random() * 4)){
		case 1:
			cellText.style = "color: rgb(" + color1 + ", " + color2 + ", " + color3 + ")";
			for (var i = 0; i < cellText.children.length; i++)
				cellText.children[i].style = "color: rgb(" + color1 + ", " + color2
					+ ", " + color3 + ")";
			break;
		case 2:
			cellText.style = "background-color: rgb(" + color1 + ", " + color2
				+ ", " +color3 + ")";
			for (var i = 0; i < cellText.children.length; i++)
				cellText.children[i].style = "background-color: rgb(" + color1 + ", " + color2
					+ ", " + color3 + ")";
			break;
		case 3:
			cellText.style = 'font-size: ' + Math.ceil(Math.random() * 11 + 14)+ 'pt;';
			for (var i = 0; i < cellText.children.length; i++)
				cellText.children[i].style = 'font-size: ' + Math.ceil(Math.random() * 11 + 14)
			+ 'pt;';
			break;
		case 4:
			cellText.remove();
			break;
	}
}

function deleteThis(){
	for (var i = 0; i < document.getElementsByTagName("body")[0].children.length; i++)
		document.getElementsByTagName("body")[0].children[i].remove();
	document.getElementsByTagName("body")[0].innerHTML="<form> Введите количество строк:"
		+ '<input type="number" id="row"> Введите количество столбцов:'
		+ '<input type="number" id="column"><button onclick="createTable()">Submit</button>'
		+ '<script type="text/javascript" src="tasks2.js"></script></form>';
}

function createTable(){
	let row = document.getElementById('row').value;
	let column = document.getElementById('column').value;
	let changeWidthText = "Submit";

	document.write("<div> Change width: <input type='text' onInput='changeWidthText()'></input>"
		+ "<select onChange='changeWidthText()'><option>solid</option><option>dotted</option>"
		+ "<option>dashed</option><option>double</option></select><input type='button'"
		+ "onclick='changeWidth()' value=changeWidthText></input></div>");
	document.write("<div> Add header: <input type='text'></input>"
		+ "<input type='button' onclick='addHeader()' value=Add header></input></div>");
	document.write("<div> Delete row: <input type='text'></input>"
		+ "<input type='button' onclick='deleteRow()' value=Delete row></input></div>");
	document.write("<div><input type='button' onclick='magic()' value=Magic></input></div>");
	document.write("<div><input type='button' onclick='deleteThis()' value=Delete></input></div>");
	
	for (var i = 0; i < document.getElementsByTagName("div").length; i++) {
		document.getElementsByTagName("div")[i].style = "float:left";
	}

	let tbl = document.createElement("table");
	let tblBody = document.createElement("tbody");

	for (var j = 0; j < row; j++) {
		let row = document.createElement("tr");
		for (var i = 0; i < column; i++) {
			let cell = document.createElement("td");
			let cellText = document.createElement("input", "type=textarea");
			let cellText2 = document.createElement("button");
			cellText2.setAttribute("onClick", "saveText("+j+", "+i+")");
			cellText2.innerHTML = "Save";
			cell.appendChild(cellText);
			cell.appendChild(cellText2);
			row.appendChild(cell);
		}
		tblBody.appendChild(row);
	}

	tbl.appendChild(tblBody);
	document.getElementsByTagName('body')[0].appendChild(tbl);
	tbl.setAttribute('border', '2');
}