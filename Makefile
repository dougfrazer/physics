default: apple
apple:
	g++ -D__APPLE__ -g src/*.cpp -framework OpenGL -framework GLUT -Iinclude

linux:
	g++ -D__LINUX__ -g src/*.cpp -lglut -lGL -lGLU -Iinclude
