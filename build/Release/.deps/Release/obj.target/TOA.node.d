cmd_Release/obj.target/TOA.node := g++ -shared -pthread -rdynamic -m64  -Wl,-soname=TOA.node -o Release/obj.target/TOA.node -Wl,--start-group Release/obj.target/TOA/src/main.o Release/obj.target/TOA/src/Line.o Release/obj.target/TOA/src/Position.o Release/obj.target/TOA/src/ConcavetToConvex.o Release/obj.target/TOA/src/PointInPolygonAlgorithm.o Release/obj.target/TOA/src/TimeOptimizationAlgorithm.o -Wl,--end-group 