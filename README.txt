To build the executable, run "make".
The executable will be named "marched_fractals".

Running this program will generate a fractal using the Fractal Flames procedure and output a model of it through the Marching Cubes algorithm.

To control the created model, the following arguments can be specified when running "./marched_fractals".

Command Usage:
	-o [string]										Name to the output model as (exclude extension)
	-i [int]										The number of iterations to run for the fractal
	-dim [int] [int] [int]							xyz dimensions of fractal model
	-weights [float] [float] [float] [float]		Weights for fractal functions
	-r [float]										Resolution of the model

Depending on the complexity of the model dictated by iterations and resolution, computation time can be on the order of seconds to minutes.