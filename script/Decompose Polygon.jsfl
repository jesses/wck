/*

Adapted from: 
http://www.ewjordan.com/earClip/
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
		if(contour.orientation == -1) {
			var he = contour.getHalfEdge();
	
			var iStart = he.id;
			var id = 0;
			var x = [];
			var y = [];
			while (id != iStart)
			{
				// see if the edge is linear
				var edge = he.getEdge();
				var vrt = he.getVertex();
				
				x.push(parseFloat(vrt.x));
				y.push(parseFloat(vrt.y));
				
				he = he.getNext();
				id = he.id;
			}			
			
			fl.drawingLayer.beginDraw(true);
			fl.drawingLayer.beginFrame();
			fl.drawingLayer.setColor('#0000ff');
			var triangles = triangulatePolygon(x,y,x.length);
			fl.drawingLayer.setColor('#ff0000');

			var polys = polygonizeTriangles(triangles);
			if(polys) {
				data = '';
				for(var i = 0; i < polys.length; i++) {
					poly_verts = '';
					fl.drawingLayer.moveTo(polys[i].x[0], polys[i].y[0]);
					for(var j = 0; j < polys[i].x.length; j++) {
						j2 = (j == polys[i].x.length - 1) ? 0 : j + 1;
						fl.drawingLayer.lineTo(polys[i].x[j2], polys[i].y[j2]);
						if(poly_verts != '') poly_verts += ', ';
						poly_verts += '[' + polys[i].x[j].toString() + ', ' + polys[i].y[j].toString() + ']';
					}
					if(data != '') data += ', ';
					data += '[' + poly_verts + ']';
				}
				fl.trace('ARRAY STRUCTURE: [polygons][verticies][x,y]')
				fl.trace('[' + data + ']');
			}
			
			fl.drawingLayer.endFrame();
			fl.drawingLayer.endDraw();
			
		}
	}
	
	
	
	
	
	
	
	
	
	
	
function Triangle(x1, y1, x2, y2, x3, y3) {
  
  this.x = [];
  this.y = [];
  
  var dx1 = x2-x1;
  var dx2 = x3-x1;
  var dy1 = y2-y1;
  var dy2 = y3-y1;
  var cross = (dx1*dy2)-(dx2*dy1);
  var ccw = (cross>0);
  if (ccw){
    this.x[0] = x1; this.x[1] = x2; this.x[2] = x3;
    this.y[0] = y1; this.y[1] = y2; this.y[2] = y3;
	} else{
		this.x[0] = x1; this.x[1] = x3; this.x[2] = x2;
		this.y[0] = y1; this.y[1] = y3; this.y[2] = y2;
	}
	
	this.isInside = function(_x, _y){
		var vx2 = _x-this.x[0]; var vy2 = _y-this.y[0];
		var vx1 = this.x[1]-this.x[0]; var vy1 = this.y[1]-this.y[0];
		var vx0 = this.x[2]-this.x[0]; var vy0 = this.y[2]-this.y[0];
		
		var dot00 = vx0*vx0+vy0*vy0;
		var dot01 = vx0*vx1+vy0*vy1;
		var dot02 = vx0*vx2+vy0*vy2;
		var dot11 = vx1*vx1+vy1*vy1;
		var dot12 = vx1*vx2+vy1*vy2;
		var invDenom = 1.0 / (dot00*dot11 - dot01*dot01);
		var u = (dot11*dot02 - dot01*dot12)*invDenom;
		var v = (dot00*dot12 - dot01*dot02)*invDenom;
		
		return ((u>0)&&(v>0)&&(u+v<1));    
	}
	
}


















function triangulatePolygon(xv, yv, vNum){
  if (vNum < 3) return null;
  
  var buffer = [];
  var bufferSize = 0;
  xrem = [];
  yrem = [];
  for (var i=0; i<vNum; ++i){
    xrem[i] = xv[i];
    yrem[i] = yv[i];
  }

  while (vNum > 3){
    //Find an ear
    var earIndex = -1;
    for (var i=0; i<vNum; ++i){
      if (isEar(i,xrem,yrem)) {
        earIndex = i;
        break;
      }
    }

    //If we still haven't found an ear, we're screwed.
    //The user did Something Bad, so return null.
    //This will probably crash their program, since
    //they won't bother to check the return value.
    //At this we shall laugh, heartily and with great gusto.
    if (earIndex == -1) {
    	//fl.trace('NO EAR FOUND!');
    	return null;
    }
    
    //Clip off the ear:
    //  - remove the ear tip from the list
    
    //Opt note: actually creates a new list, maybe
    //this should be done in-place instead.  A linked
    //list would be even better to avoid array-fu.
    --vNum;
    var newx = [];
    var newy = [];
    var currDest = 0;
    for (var i=0; i<vNum; ++i){
      if (currDest == earIndex) ++currDest;
      newx[i] = xrem[currDest];
      newy[i] = yrem[currDest];
      ++currDest;
    }
    
    //  - add the clipped triangle to the triangle list
    var under = (earIndex==0)?(xrem.length-1):(earIndex-1);
    var over = (earIndex==xrem.length-1)?0:(earIndex+1);
    var toAdd = new Triangle(xrem[earIndex],yrem[earIndex],xrem[over],yrem[over],xrem[under],yrem[under]);
    buffer[bufferSize] = toAdd;
    ++bufferSize;
        
    //  - replace the old list with the new one
    xrem = newx;
    yrem = newy;
  }
  var toAdd = new Triangle(xrem[1],yrem[1],xrem[2],yrem[2],xrem[0],yrem[0]);
  buffer[bufferSize] = toAdd;
  ++bufferSize;
  
  var res = [];
  for (var i=0; i<bufferSize; i++){
    res[i] = buffer[i];
  }
  return res;
}

function polygonizeTriangles(triangulated){
  var polys;
  var polyIndex = 0;
  
  if (triangulated == null){
    return null;
  } else{
    polys = [];
    var covered = [];
    for (var i=0; i<triangulated.length; i++){
      covered[i] = false;
    }
    
    var notDone = true;
    
    while(notDone){
      var currTri = -1;
      for (var i=0; i<triangulated.length; i++){
        if (covered[i]) continue;
        currTri = i;
        break;
      }
      if (currTri == -1){
        notDone = false;
      } else{
        var poly = new Polygon(triangulated[currTri]);
        covered[currTri] = true;
        for (var i=0; i<triangulated.length; i++){
          if (covered[i]) continue;
          var newP = poly.add(triangulated[i]);
          if (newP == null) continue;
          if (newP.isConvex()){
            poly = newP;
            covered[i] = true;
          }
        }
        polys[polyIndex] = poly;
        polyIndex++;
      }
    }
  }
  var ret = [];
  for (var i=0; i<polyIndex; i++){
    ret[i] = polys[i];
  }
  return ret;
}

//Checks if vertex i is the tip of an ear
function isEar(i, xv, yv){
  var dx0,dy0,dx1,dy1;
  dx0=dy0=dx1=dy1=0.0;
  if (i >= xv.length || i < 0 || xv.length < 3){
    return false;
  }
  var upper = i+1;
  var lower = i-1;
  if (i == 0){
    dx0 = xv[0] - xv[xv.length-1]; 
    dy0 = yv[0] - yv[yv.length-1];
    dx1 = xv[1] - xv[0]; 
    dy1 = yv[1] - yv[0];
    lower = xv.length-1;
  } else if (i == xv.length-1){
    dx0 = xv[i] - xv[i-1]; 
    dy0 = yv[i] - yv[i-1];
    dx1 = xv[0] - xv[i]; 
    dy1 = yv[0] - yv[i];
    upper = 0;
  } else{
    dx0 = xv[i] - xv[i-1]; 
    dy0 = yv[i] - yv[i-1];
    dx1 = xv[i+1] - xv[i]; 
    dy1 = yv[i+1] - yv[i];
  }
  var cross = (dx0*dy1)-(dx1*dy0);
  if (cross > 0) return false;
  var myTri = new Triangle(xv[i],yv[i],xv[upper],yv[upper],xv[lower],yv[lower]);
  
  /*fl.drawingLayer.moveTo(xv[i],yv[i]);
  fl.drawingLayer.lineTo(xv[upper],yv[upper]);
  fl.drawingLayer.lineTo(xv[lower],yv[lower]);
  fl.drawingLayer.lineTo(xv[i],yv[i]);
*/
  for (var j=0; j<xv.length; ++j){
    if (!(j==i || j == lower || j == upper)) {
    	if (myTri.isInside(xv[j],yv[j])) return false;
  	}
  }
  return true;
}







function Polygon(_x, _y) {
  
  if(!_y) {
  	_y = _x.y;
  	_x = _x.x;
  }
  
	this.nVertices = _x.length;
//    println("length "+nVertices);
	this.x = []
	this.y = []
	for (var i=0; i<this.nVertices; ++i){
		this.x[i] = _x[i];
		this.y[i] = _y[i];
	}
	
	this.set = function(p){
		this.nVertices = p.nVertices;
		this.x = new float[this.nVertices];
		this.y = new float[this.nVertices];
		for (var i=0; i<this.nVertices; ++i){
			this.x[i] = p.x[i];
			this.y[i] = p.y[i];
		}
	}

	/*
	 * Assuming the polygon is simple, checks
	 * if it is convex.
	 */
	this.isConvex = function(){
		var isPositive = false;
		for (var i=0; i<this.nVertices; ++i){
			var lower = (i==0)?(this.nVertices-1):(i-1);
			var middle = i;
			var upper = (i==this.nVertices-1)?(0):(i+1);
			var dx0 = this.x[middle]-this.x[lower];
			var dy0 = this.y[middle]-this.y[lower];
			var dx1 = this.x[upper]-this.x[middle];
			var dy1 = this.y[upper]-this.y[middle];
			var cross = dx0*dy1-dx1*dy0;
			//Cross product should have same sign
			//for each vertex if poly is convex.
			var newIsP = (cross>0)?true:false;
			if (i==0){
				isPositive = newIsP;
			} else if (isPositive != newIsP){
				return false;
			}
		}
		return true;
	}

	/*
	 * Tries to add a triangle to the polygon.
	 * Returns null if it can't connect properly.
	 * Assumes bitwise equality of join vertices.
	 */
	this.add = function(t){
		//First, find vertices that connect
		var firstP = -1; 
		var firstT = -1;
		var secondP = -1; 
		var secondT = -1;
	//    println("nVertices: "+this.nVertices);
		for (var i=0; i < this.nVertices; i++){
			if (t.x[0] == this.x[i] && t.y[0] == this.y[i]){
	//        println("found p0");
				if (firstP == -1){
					firstP = i; firstT = 0;
				} else{
					secondP = i; secondT = 0;
				}
			} else if (t.x[1] == this.x[i] && t.y[1] == this.y[i]){
	//        println("found p1");
				if (firstP == -1){
					firstP = i; firstT = 1;
				} else{
					secondP = i; secondT = 1;
				}
			} else if (t.x[2] == this.x[i] && t.y[2] == this.y[i]){
	//        println("found p2");
				if (firstP == -1){
					firstP = i; firstT = 2;
				} else{
					secondP = i; secondT = 2;
				}
			} else {
	//        println(t.x[0]+" "+t.y[0]+" "+t.x[1]+" "+t.y[1]+" "+t.x[2]+" "+t.y[2]);
	//        println(x[0]+" "+y[0]+" "+x[1]+" "+y[1]);
			}
		}
		//Fix ordering if first should be last vertex of poly
		if (firstP == 0 && secondP == this.nVertices-1){
			firstP = this.nVertices-1;
			secondP = 0;
		}
		
		//Didn't find it
		if (secondP == -1) return null;
		
		//Find tip index on triangle
		var tipT = 0;
		if (tipT == firstT || tipT == secondT) tipT = 1;
		if (tipT == firstT || tipT == secondT) tipT = 2;
		
		var newx = [];
		var newy = [];
		var currOut = 0;
		for (var i=0; i<this.nVertices; i++){
			newx[currOut] = this.x[i];
			newy[currOut] = this.y[i];
			if (i == firstP){
				++currOut;
				newx[currOut] = t.x[tipT];
				newy[currOut] = t.y[tipT];
			}
			++currOut;
		}
		return new Polygon(newx,newy);
	}
}