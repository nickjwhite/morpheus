function printjson(j) {
	var i;
	var t = "";

        for(i in j) {
                t += "<dt>" + i + "</dt><dd>";
                if(j[i] instanceof Object) {
                        t += "<dl>" + printjson(j[i]) + "</dl>";
                } else {
                        t += j[i];
                }
                t += "</dd>";
        }

        return t;
}

function analyseme(e) {
	var a;
	var d;
	var j;
	var s;
	var x;

	s = e.currentTarget.textContent.trim().toLowerCase().replace(/[\";\.,]/, "");

	j = JSON.parse(analysis_as_json(s));

	d = document.createElement("div");
	d.id = "morphanalysis";
	d.style = "left: " + e.clientX + "px; top: " + e.clientY + "px;";

	x = document.createElement("span");
	x.style = "float: right; font-family: sans-serif;";
	x.textContent = "x";
	x.id = "x";
	x.addEventListener("click", function(e) {
		var t;
		if((t = document.getElementById("morphanalysis")) != null) {
			document.body.removeChild(t);
		}
	}, false);
	d.appendChild(x);

	x = document.createElement("span");
	x.innerHTML = printjson(j);
	d.appendChild(x);

	if((a = document.getElementById("morphanalysis")) != null) {
		document.body.replaceChild(d, a);
	} else {
		document.body.appendChild(d);
	}
}

function loadanalyser() {
	var a;
	var i;
	var n;
	var s;
	for(i of document.getElementsByTagName("p")) {
		n = document.createElement("p");
		for(a of i.textContent.split(" ")) {
			s = document.createElement("span");
			s.addEventListener("click", analyseme);
			s.textContent = a + " ";
			n.appendChild(s);
		}
		document.body.replaceChild(n, i);
	}
}
