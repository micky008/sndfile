# sndfile

1) installer libsndfile-wrapper-rCURRENT dnas son repo local (copier coller la ligne en dessous).

`mvn install:install-file -Dfile=./libsndfile-wrapper-rCURRENT.jar -DgroupId=edu.cornell -DartifactId=libsndfile-wrapper -Dversion=0.1 -Dpackaging=jar -DgeneratePom=true`

2) bien pensé a ajouter le folder libs/ dnas la variable d'environement PATH

et le programme se lance.
