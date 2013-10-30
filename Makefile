default:
apple:
	g++ -D__APPLE__ -g *.cpp -framework OpenGL -framework GLUT

linux:
	g++ -D__LINUX__ -g *.cpp -lglut -lGL -lGLU
