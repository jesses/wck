/*

Sets a component property on multiple selected components

*/


var sel = fl.getDocumentDOM().selection;
var pname = prompt('What property do you want to set?');
if(pname) {
	var pvalue = prompt('What value do you want to set it to?');
	if(pvalue != null) {
		for(var i = 0; i < sel.length; ++i) {
			var s = sel[i];
			if(s.parameters) {
				var j = 0;
				var p = null;
				while(j < s.parameters.length && !p) {
					if(s.parameters[j].name == pname) {
						p = s.parameters[j];
					}
					j++;
				}
				if(p) {
					p.value = pvalue;
				}
			}
		}
	}
}