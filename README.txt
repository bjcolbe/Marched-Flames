To build the executable, run "make". The executable will be named "marched_fractals".

Running this "marched_fractals" will generate a fractal using the Fractal Flames procedure and output a model of it through the Marching Cubes algorithm.
Default arguments will save the model to "model.obj" and the color information to "model.mtl". This must be viewed with 3D software such as Blender.

To control the created model, the following arguments can be specified when running "./marched_fractals".

Command Usage:
	-o [string]								Name to the output model as (exclude extension)
	-i [int]								The number of iterations to run for the fractal
	-dim [int] [int] [int]							xyz dimensions of fractal model
	-weights [float] [float] [float] [float]				Weights for fractal functions
	-r [float]								Resolution of the model
	-f [fractal | sincos | sin | cos]					Determines the method used to create vertices
	-sdim [int] [int] [int]							Specify the marching cubes space to reduce computation time

Depending on the complexity of the model dictated by iterations and resolution, computation time can be on the order of seconds to minutes.

Sample Usage
	./marched_fractals -r 15 -f sincos -dim 10 4 10
		Show a visualization of f(x,z) = sin(x) + cos(z)
	./marched_fractals
		Show a fractal visualization
	./marched_fractals -i 10000 -r 20 -dim 10000 10000 10000 -device cpu
		Shows a high resolution fractal (uses cpu computation)