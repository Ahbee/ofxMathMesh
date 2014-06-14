#ofxMathMesh

ofxMathMesh allows you to create ofMeshs form mathematical equations. You can create meshes from 3dFunctions, 2dFunctions, Parametric Surfaces and Parametric Curves

you can see a small demo here - [https://vimeo.com/98150899](https://vimeo.com/98150899)

##Features

* Creates Meshes from 3d Functions, 2d Functions, Parametric Surfaces and Parametric Curves

* Auto Calculates Surface Normals 

* Customize Colors and TexCoords on both Front and Back Sides

* Draw wireframe and point meshes with customized color. 

* Control bounds and resolution of your mesh


##How to Use

####Step 1 - create a subclass

you can subclass from anyone of these parents:

* `ofx3dFunction` - if your equation is of the form y = f(x,z)
* `ofx2dFuntion` - if your equation is of the form y = f(x)
* `ofxParametricSurface` -  if your equation is of the form r = r(u,v)
* `ofxParametrixCurve` - if your equation is of the form r = r(t)

For Example to create a Sphere(which is a parametric Surface) you do this:

```
class Sphere:public ofxParametricSurface
```
####Step 2 - override
in order to customize your mesh you must override one or more  of the following functions:

* `valueForPoint` - mandatory     
* `colorForPoint` - optional
* `texCoordForPoint` - optional 
* `backColorForPoint`- optional
* `backTexCoordForPoint` - optional

For example to create a Sphere thats half red half blue you do this:

```
class Sphere:public ofxParametricSurface {
public:
    ofPoint valueForPoint(float u, float v){
        float x = cos(u) * sin(v);
        float y = sin(u) * sin(v);
        float z = cos(v);
        return ofPoint(x,y,z);
    }
    
     ofFloatColor colorForPoint(float u,float v,ofPoint value){
        if (u > M_PI && u < 2*M_PI - .0001) {
            return ofFloatColor::blue;
        }else{
            return ofFloatColor::red;
        }
    }

};

```

####Step 4 - call setup

`setup` must be called in order to create a mesh. In `setup` you specify the bounds and step of your equation. Lower step means higher resolutions.
For example to set up a full sphere you do this:

```
Sphere mySphere;
mySphere.setup(0, 2*M_PI, 0, M_PI, .1, .1);
```
here *u* goes form 0 to 2PI, *v* goes from 0 to PI, and uStep = .1, and vStep = .1. Note that if you want to change the bounds of the equation after setup,you cannot go outside the initial setup bounds.

####Step 5 - call draw 

to draw your equation you call either `draw ` , `drawWireFrame`, or `drawPoints`

```
    mySphere.draw(true, false);
    mySphere.drawWireFrame(true);
    mySphere.drawPoints(true);
```
The 1st argument specifies whether you over-rid `colorForPoint`. The 2nd argument specifies whether you over-rid `texCoordForPoint`. The results looks like this

![draw](https://farm4.staticflickr.com/3868/14408118032_2d009e8fee_m.jpg)

![drawWireFrame](https://farm3.staticflickr.com/2905/14429600343_d3c075614b_m.jpg)

![drawPoints](https://farm4.staticflickr.com/3887/14222986297_d353a68c9f_m.jpg)



####Step 6 - reloading

whenever to you update your equation, colors, texcoords,or bounds.You want to call `reload()`

```
mySphere.reload()
``` 
this is useful for animation and live updates of your equation. If you are reloading constantly its a good idea to call this in the `update()` function.

####Step 7 - drawing two sided

if you want to have different colors and textures on front and backside, you need  to implement either `backColorForPoint` or `backTexCoordForPoint`. Then call the functions `drawFrontFaces` and `drawBackFaces` 

For example to draw a sphere that is red and blue on the front and green and yellow on the back, you would do this.

```

class Sphere:public ofxParametricSurface {
public:
    ofPoint valueForPoint(float u, float v){
        float x = cos(u)* sin(v);
        float y = sin(u)* sin(v);
        float z = cos(v);
        return ofPoint(x,y,z);
    }
    
    ofFloatColor colorForPoint(float u,float v,ofPoint value){
        if (u > M_PI && u < 2*M_PI - .0001) {
            return ofFloatColor::blue;
        }else{
            return ofFloatColor::red;
        }
    }
    
    ofFloatColor backColorForPoint(float u,float v,ofPoint value){
        if (u > M_PI && u < 2*M_PI - .0001) {
            return ofFloatColor::green;
        }else{
            return ofFloatColor::yellow;
        }
    }
};

```


```
 	mySphere.setUMax(1.5 *M_PI);
    mySphere.reload();
    mySphere.drawFrontFaces(true, false);
    mySphere.drawBackFaces(true, false);
```
this is the resulting sphere. Draw part open(uMax set to 1.5PI)

![2Sided](https://farm4.staticflickr.com/3903/14222817028_4a919cdca0_m.jpg)

#### step 8 - getting an ofMesh

You retrieve an ofMesh from your by equation by calling `getMesh()` or `getBackMesh`

```
    // returns a mesh which with vertices,frontNormals,frontColors,and frontTexCoords
    ofMesh myMesh = mySphere.getMesh();
    
    // returns a mesh which with vertices,backNormals,backColors,and backTexCoords
    ofMesh myBackMesh = mySphere.getBackMesh();
```  

#### step 9 - normals

Normals are auto calculated using these [formulas](http://mathworld.wolfram.com/NormalVector.html), if you want to draw the normals you can use the functions `drawNormals(float length)` or `drawFaceNormals(float length)`

##Extras
There are a lot more things you can do with this add-on so if you have any  questions, you can ask me on the [forum thread](http://forum.openframeworks.cc/t/ofxmathmesh-create-meshes-from-mathematical-equations/16099)



  



 















 


