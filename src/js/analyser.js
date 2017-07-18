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

	s = e.currentTarget.textContent.trim().toLowerCase();

	j = JSON.parse(analysis_as_json(s));
	
	d = document.createElement("div");
	d.id = "morphanalysis";
	d.style = "display: block; position: absolute; border: thin solid black;";
	d.innerHTML = printjson(j);

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
