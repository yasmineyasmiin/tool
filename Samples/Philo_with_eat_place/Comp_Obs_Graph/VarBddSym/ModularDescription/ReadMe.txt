Ici,les réseaux sont décomposés en n sous-réseaux se partageant un ensemble de transitions.
Les variables BDD sont affectées à des places symétriques du système 
(i.e. v1--->Idle1, Idle2, v2--->...Eat1,Eat2, ... etc).
L'entrée de l'outil de synchronisation à la volé des graphes d'observations correspondants
aux sous-réseaux et sous la forme suivante:
SR1 INT1 SR2 INT2 ....SRn INTn
où 
1. SRi (i=1...n) est le fichier contenant la description du sous-réseaux Ri
1. INTi (i=1...n) est le fichier contenant les transitions de l'interface du 
sous-réseaux Ri dans une décomposition du modèle global en n sous-réseaux.
