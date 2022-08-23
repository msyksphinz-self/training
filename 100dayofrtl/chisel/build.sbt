// build.sbt
scalaVersion := "2.13.8"
addCompilerPlugin("edu.berkeley.cs" % "chisel3-plugin" % "3.5.3" cross CrossVersion.full)
libraryDependencies += "edu.berkeley.cs" %% "chisel3" % "3.5.3"
// We also recommend using chiseltest for writing unit tests
libraryDependencies += "edu.berkeley.cs" %% "chiseltest" % "0.5.3" % "test"
