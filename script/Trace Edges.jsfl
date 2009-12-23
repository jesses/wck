/*

Adapted from: 
http://livedocs.adobe.com/flash/9.0/main/wwhelp/wwhimpl/common/html/wwhelp.htm?context=LiveDocs_Parts&file=00003808.html#wp461362

*/
			
	var selArray = fl.getDocumentDOM().selection;
	fl.trace( "selection length: " + selArray.length );

	var elt = selArray[0];
	fl.trace( "selected element" + elt );
	fl.trace( "element type: " + elt.elementType );
	
	var contourArray = elt.contours;
	//fl.trace("contour array length: " + contourArray.length);

	for (i=0;  i<contourArray.length;  i++)
	{
		var contour = contourArray[i];
		if(contour.orientation == 1) {
			var he = contour.getHalfEdge();
	
			var iStart = he.id;
			var id = 0;

			fl.drawingLayer.beginDraw(true);
			fl.drawingLayer.beginFrame();
			fl.drawingLayer.setColor('#0000ff');
			fl.drawingLayer.setColor('#ff0000');
			data = '';
			
			var first = true;
			
			while (id != iStart)
			{
				// see if the edge is linear
				var edge = he.getEdge();
				var vrt = he.getVertex();
				
				if(first) {
					fl.drawingLayer.moveTo(vrt.x, vrt.y);
					first = false;
				}
				else {
					data += ',';
					fl.drawingLayer.lineTo(vrt.x, vrt.y);
				}
				
				data += '['+ vrt.x +','+ vrt.y +']';
				
				he = he.getNext();
				id = he.id;
			}			
			
			fl.trace('EDGES: ['+ data +']');
			
			fl.drawingLayer.endFrame();
			fl.drawingLayer.endDraw();
			
		}
	}