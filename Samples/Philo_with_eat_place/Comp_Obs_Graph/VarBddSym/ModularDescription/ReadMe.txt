Ici,les r�seaux sont d�compos�s en n sous-r�seaux se partageant un ensemble de transitions.
Les variables BDD sont affect�es � des places sym�triques du syst�me 
(i.e. v1--->Idle1, Idle2, v2--->...Eat1,Eat2, ... etc).
L'entr�e de l'outil de synchronisation � la vol� des graphes d'observations correspondants
aux sous-r�seaux et sous la forme suivante:
SR1 INT1 SR2 INT2 ....SRn INTn
o� 
1. SRi (i=1...n) est le fichier contenant la description du sous-r�seaux Ri
1. INTi (i=1...n) est le fichier contenant les transitions de l'interface du 
sous-r�seaux Ri dans une d�composition du mod�le global en n sous-r�seaux.
