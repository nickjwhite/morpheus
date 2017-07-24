function printjson(j) {
	var a;
	var i;
	var n;
	var t = "";

        for(i in j) {
		t += "<h3>" + i + "</h3>";

		for(n of j[i]) {
			t += "<p>";
			for(a in n) {
				t += a + ": " + n[a] + "<br />";
			}
			t += "</p>";
		}
        }

        return t;
}

function analyseme(e) {
	var a;
	var d;
	var j;
	var s;
	var x;

	s = e.currentTarget.textContent.trim().replace(/[\";\.,]/, "");

	j = JSON.parse(MorpheusAnalysis(s));

	d = document.createElement("div");
	d.id = "morphanalysis";
	d.style = "left: " + e.clientX + "px; top: " + e.clientY + "px;";

	x = document.createElement("span");
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
		for(l of i.textContent.split("\n")) {
			for(a of l.split(" ")) {
				s = document.createElement("span");
				s.addEventListener("click", analyseme);
				s.textContent = a + " ";
				n.appendChild(s);
			}
			s = document.createElement("br");
			n.appendChild(s);
		}
		document.body.replaceChild(n, i);
	}
}
