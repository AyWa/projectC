#include "LabelRosenfeld.hpp"

/* Constructeur par defaut */
LabelRosenfeld::LabelRosenfeld() {
}

LabelRosenfeld::~LabelRosenfeld() {
}

/* Fonctions communes */
uint32_t LabelRosenfeld::FindRoot(uint32_t* T, uint32_t i) {
    uint32_t r;
    r = i;
    while (T[r]<r) {
        r = T[r];
    }
    return r;
}
void LabelRosenfeld::SetRoot(uint32_t* T, uint32_t i, uint32_t r) {
    uint32_t j;

    while (T[i]<i) {
        j = T[i];
        i = j;
    }
    T[i] = r;
}
void LabelRosenfeld::updateTable(uint32_t* T, uint32_t e, uint32_t epsillon) {
    uint32_t m;

    if ((!e) || (e == epsillon)) return;

    while (T[e] != epsillon) {
        m = T[e];
        T[e] = epsillon;
        e = m;
    }
}

uint32_t LabelRosenfeld::ui32MinNonNul2(uint32_t a, uint32_t b) {
    uint32_t m;

    if (a) {
        if (b) {
            if (a<b) m = a;
            else m =b;
        } else {
            m = a;
        }
    } else {
        m = b;
    }
    return m;
}
uint32_t LabelRosenfeld::ui32MinNonNul3(uint32_t a, uint32_t b, uint32_t c) {
    uint32_t m32 = 0xffffffff;
    uint32_t m = m32;

    if (a) m = a;
    if (b && (b<m)) m = b;
    if (c && (c<m)) m = c;

    if (m==m32) m = 0;
    return m;
}
uint32_t LabelRosenfeld::ui32MinNonNul4(uint32_t a0, uint32_t b0, uint32_t a1, uint32_t b1) {
    uint32_t m, m0,m1;

    m0 = ui32MinNonNul2(a0, b0);
    m1 = ui32MinNonNul2(a1, b1);
    m  = ui32MinNonNul2(m0, m1);

    return m;
}

/* Autres fonctions utiles */
uint32_t LabelRosenfeld::solvePackTable(uint32_t* T, uint32_t ne) {
    uint32_t e;
    uint32_t na; // ancetre packe

    na = 0;
    for (e=1; e<=ne; e++) {
        if (e != T[e]) {
            T[e] = T[T[e]];
        } else {
            T[e] = ++na;
        }
        //std::cout << e << std::endl;
    }
    return na;
}
void LabelRosenfeld::updateLabel(uint32_t **E, int i0, int i1, int j0, int j1, uint32_t* T) {
    int i, j;

    for (i=i0; i<i1; i++) {
        for (j=j0; j<j1; j++) {
            E[i][j] = T[E[i][j]];
        }
    }
}

uint32_t LabelRosenfeld::line0Labeling4C(unsigned char **X, int i, uint32_t **E, uint32_t* T, int largeur, uint32_t ne) {
    int j;
    unsigned char x;
    uint32_t e4, r4;

    // Premier point
    x = X[i][0];
    if (x) {
        E[i][0] = ++ne;
    } else {
        E[i][0] = 0;
    }

    for (j=1; j<largeur; j++) {
        x = X[i][j];
        if (x)  {
            e4 = E[i  ][j-1];

            if (e4 == 0) {
                E[i][j] = ++ne;
            } else {
                r4 = FindRoot(T, e4);
                E[i][j] = r4;
            }
        }
    }
    return ne;
}
uint32_t LabelRosenfeld::lineLabeling4C(unsigned char **X, int i, uint32_t **E, uint32_t* T, int largeur, uint32_t ne) {
    int j;
    unsigned char x;
    uint32_t epsillon;
    uint32_t e2, e4;
    uint32_t r2, r4;

    // premier point de la ligne
    x = X[i][0];

    if (x) {
        e2 = E[i-1][0];
        if (e2) {
            E[i][0] = e2;
        } else {
            E[i][0] = ++ne;
        }
    }

    for (j=1; j<largeur; j++) {
        x = X[i][j];
        if (x)  {
            e2 = E[i-1][j  ];
            e4 = E[i  ][j-1];

            if ( (e2 == 0) && (e4 == 0) ) {
                E[i][j] = ++ne;
            } else {
                if (e2 == e4) {
                    E[i][j] = e2;
                } else {
                    r2 = FindRoot(T, e2);
                    r4 = FindRoot(T, e4);
                    epsillon = ui32MinNonNul2(r2, r4);

                    if ((e2) && (e2 != epsillon)) SetRoot(T, e2, epsillon);
                    if ((e4) && (e4 != epsillon)) SetRoot(T, e4, epsillon);
                    E[i][j] = epsillon;
                }
            }
        }
    }
    return ne;
}

uint32_t LabelRosenfeld::line0Labeling8C(unsigned char **X, int i, uint32_t **E, uint32_t* T, int largeur, uint32_t ne) {
    int j;
    unsigned char x;
    uint32_t e4,e;

    // Premier point
    x = X[i][0];
    if (x) {
        E[i][0] = ++ne;
    } else {
        E[i][0] = 0;
    }

    for (j=1; j<largeur; j++) {
        x = X[i][j];
        if (x)  {
            e4 = E[i][j-1];
            if (e4) {
                e = FindRoot(T,e4);
            } else {
                e = ++ne;
            }
            E[i][j] = e;
        }
    }
    return ne;
}
uint32_t LabelRosenfeld::lineLabeling8C(unsigned char **X, int i, uint32_t **E, uint32_t* T, int largeur, uint32_t ne) {
    int j;
    unsigned char x;
    uint32_t epsillon;
    uint32_t e1, e2, e3, e4;
    uint32_t r1, r2, r3, r4;

    // premier point de la ligne
    x = X[i][0];
    if (x) {
        e2 = E[i-1][0];
        e3 = E[i-1][1];

        if ( (e2==0) && (e3==0)) {
            E[i][0] = ++ne;
        } else {
            if (e2==e3) {
                E[i][0] = e2;
            } else {
                r2 = FindRoot(T,e2);
                r3 = FindRoot(T,e3);

                epsillon = ui32MinNonNul2(r2, r3);

                if ((e2) && (e2 != epsillon)) SetRoot(T, e2, epsillon);
                if ((e3) && (e3 != epsillon)) SetRoot(T, e3, epsillon);

                E[i][0] = epsillon;
            }
        }
    }

    for (j=1; j<largeur-1; j++) {
        x = X[i][j];
        if (x)  {
            e1 = E[i-1][j-1];
            e2 = E[i-1][j  ];
            e3 = E[i-1][j+1];
            e4 = E[i  ][j-1];

            // nouvel element
            if ( (e1==0) && (e2==0) && (e3==0) && (e4==0)) {
                E[i][j] = ++ne;
            } else {
                if ((e1 == e2) && (e1 == e3) && (e1 == e4)) {
                    E[i][j] = e1;
                } else {

                    r1 = FindRoot(T,e1);
                    r2 = FindRoot(T,e2);
                    r3 = FindRoot(T,e3);
                    r4 = FindRoot(T,e4);

                    epsillon = ui32MinNonNul4(r1, r2, r3, r4);

                    if ((e1) && (e1 != epsillon)) SetRoot(T, e1, epsillon);
                    if ((e2) && (e2 != epsillon)) SetRoot(T, e2, epsillon);
                    if ((e3) && (e3 != epsillon)) SetRoot(T, e3, epsillon);
                    if ((e4) && (e4 != epsillon)) SetRoot(T, e4, epsillon);

                    E[i][j] = epsillon;
                }
            }
        }
    }

    j = largeur-1;
    x = X[i][j];
    if (x)  {
        e1 = E[i-1][j-1];
        e2 = E[i-1][j  ];
        e3 = 0;
        e4 = E[i  ][j-1];

        // nouvel element
        if ( (e1==0) && (e2==0) && (e3==0) && (e4==0)) {
            E[i][j] = ++ne;

        } else {
            if ((e1 == e2) && (e1 == e3) && (e1 == e4)) {
                E[i][j] = e1;
            } else {

                r1 = FindRoot(T,e1);
                r2 = FindRoot(T,e2);
                r3 = FindRoot(T,e3);
                r4 = FindRoot(T,e4);

                epsillon = ui32MinNonNul4(r1, r2, r3, r4);

                if ((e1) && (e1 != epsillon)) SetRoot(T, e1, epsillon);
                if ((e2) && (e2 != epsillon)) SetRoot(T, e2, epsillon);
                if ((e3) && (e3 != epsillon)) SetRoot(T, e3, epsillon);
                if ((e4) && (e4 != epsillon)) SetRoot(T, e4, epsillon);

                E[i][j] = epsillon;
            }
        }
    }
    return ne;
}



/* Labelise en sequentiel */
void LabelRosenfeld::labeliseSequetiel4C(Region32& region32) {

    /* Declaration des variables */
    int i;
    uint32_t ne;
    int i0 			= 	region32.i0;
    int i1 			= 	region32.i1;
    int j0 			= 	region32.j0;
    int j1 			= 	region32.j1;
    int largeur 	= 	j1-j0;

    /* Netoyage des pr�c�dents traitements */
    region32.cleanRegions32();

    /* Premier etiquetage */
    ne = 0;
    ne = line0Labeling4C(region32.X, i0, region32.E, region32.T, largeur, ne);
    for (i=i0+1; i<i1; i++) {
        ne = lineLabeling4C(region32.X, i, region32.E, region32.T, largeur, ne);
    }

    /* R�solution des �quivalences */
    region32.neFinal = solvePackTable(region32.T, ne);

    // /* Mise � jour sur l'image */
     updateLabel(region32.E, i0, i1, j0, j1, region32.T);
    //
    // /* M�morisation du nombre d'�tiquettes */
    // region32.ne = ne;
}
void LabelRosenfeld::labeliseSequetiel8C(Region32& region32) {

    /* Declaration des variables */
    int i;
    uint32_t ne;

    int i0 			= 	region32.i0;
    int i1 			= 	region32.i1;
    int j0 			= 	region32.j0;
    int j1 			= 	region32.j1;
    int largeur 	= 	j1-j0;

    /* Netoyage des pr�c�dents traitements */
    region32.cleanRegions32();

    /* Premier etiquetage */
    ne = 0;

    ne = line0Labeling8C(region32.X, i0, region32.E, region32.T, largeur, ne);
    for (i=i0+1; i<i1; i++) {
        ne = lineLabeling8C(region32.X, i, region32.E, region32.T, largeur, ne);
    }

    /* R�solution des �quivalences */
    region32.neFinal = solvePackTable(region32.T, ne);

    /* Mise � jour sur l'image */
    updateLabel(region32.E, i0, i1, j0, j1, region32.T);

    /* M�morisation du nombre d'�tiquettes */
    region32.ne = ne;
}




/* Labelise en parall�le */
void LabelRosenfeld::labeliseParallele4C(Region32& region32) {
  /* Declaration des variables */
  int i;
  uint32_t ne;// nombre de label different
  uint32_t *ne_threads; //nombre de label thread
  int i0 			= 	region32.i0;//hauteur debut
  int i1 			= 	region32.i1;//hauteur fin
  int j0 			= 	region32.j0;//largeur debut
  int j1 			= 	region32.j1;//largeur fin
  int largeur 	= 	j1-j0; //largeur
  /* Premier etiquetage */
  //region32.X
  //i0 hauteur debut
  //region32.E
  //region32.T
  //largeur Largeur image
  //ne nombre de label different
  ne_threads=(uint32_t*)malloc(sizeof(uint32_t)*region32.Regions.size());
  //region32.Regions liste des sous regions: Region32
  /* Netoyage des pr�c�dents traitements */
  region32.cleanRegions32();
  ne = 0;
  //on fait une boucle pour chaque region: on va paralleliser cette boucle
  for(int nbRegion=0;nbRegion<region32.Regions.size();nbRegion++){
    region32.Regions[nbRegion].cleanRegions32();
    region32.Regions[nbRegion].ne=0;
    region32.Regions[nbRegion].ne = line0Labeling4C(region32.Regions[nbRegion].X, region32.Regions[nbRegion].i0, region32.Regions[nbRegion].E, region32.Regions[nbRegion].T, largeur, region32.Regions[nbRegion].ne);
    //nos thread pour gerer chaque region
    for (i=region32.Regions[nbRegion].i0+1; i<region32.Regions[nbRegion].i1; i++) {
        region32.Regions[nbRegion].ne = lineLabeling4C(region32.Regions[nbRegion].X, i, region32.Regions[nbRegion].E, region32.Regions[nbRegion].T, largeur, region32.Regions[nbRegion].ne);
    }
    std::cout << "/* message */" << std::endl;
    std::cout <<  region32.Regions[nbRegion].ne<< std::endl;
    std::cout << region32.Regions[nbRegion].ne << std::endl;
    region32.ne+=region32.Regions[nbRegion].ne;
  }
  //build all my ne_threads
  std::cout << region32.ne << std::endl;
  //uint32_t *T=(uint32_t*)malloc(sizeof(uint32_t)*region32.ne+1);
  region32.initialiseTables(region32.ne);
  int tmp3=0;
  for(int j=0;j<region32.Regions.size();j++){
    for(int tmp=0;tmp<=region32.Regions[j].ne;tmp++){
      //T[tmp+tmp3]=region32.Regions[j].T[tmp]+tmp3;
      region32.T[tmp+tmp3]=region32.Regions[j].T[tmp]+tmp3;
    }
    tmp3+=region32.Regions[j].ne;
  }
  //probleme de ligne
  //update region32.T
  //parcourir la ligne
  //s'il y a un pixelon regarde en haut
  int x,y=0;
  int aa,zz=0;
  for(int j=0;j<largeur;j++){
    x = region32.Regions[1].E[region32.Regions[1].i0][j];
    if(x){
      std::cout << "x"<< x << " hauteur: "<<region32.Regions[1].i0 << std::endl;
      y=region32.Regions[0].E[region32.Regions[0].i1-1][j];
      if(y){
        std::cout << "y" << y << " hauteur: "<<region32.Regions[0].i1-1 <<std::endl;
        //std::cout << "root:"<<  << std::endl;
        aa=region32.Regions[1].T[x];
        zz=region32.Regions[0].T[y];
        std::cout << " aa " << aa <<  std::endl;
        std::cout << " zz " << zz <<  std::endl;
        if(aa != zz) SetRoot(region32.T,FindRoot(region32.Regions[0].T,zz),FindRoot(region32.Regions[1].T,aa));
      }
    }
  }
  region32.neFinal = solvePackTable(region32.T, region32.ne);
  // /* Mise � jour sur l'image */
  updateLabel(region32.E, i0, i1, j0, j1, region32.T);
  //
  // /* M�morisation du nombre d'�tiquettes */
  // region32.ne = ne;
}

void LabelRosenfeld::labeliseParallele8C(Region32& region32) {

      /* Declaration des variables */
      int i;
      uint32_t ne;

      int i0 			= 	region32.i0;
      int i1 			= 	region32.i1;
      int j0 			= 	region32.j0;
      int j1 			= 	region32.j1;
      int largeur 	= 	j1-j0;

      /* Netoyage des pr�c�dents traitements */
      region32.cleanRegions32();

      /* Premier etiquetage */
      ne = 0;

      ne = line0Labeling8C(region32.X, i0, region32.E, region32.T, largeur, ne);
      for (i=i0+1; i<i1; i++) {
          ne = lineLabeling8C(region32.X, i, region32.E, region32.T, largeur, ne);
      }

      /* R�solution des �quivalences */
      region32.neFinal = solvePackTable(region32.T, ne);

      /* Mise � jour sur l'image */
      updateLabel(region32.E, i0, i1, j0, j1, region32.T);

      /* M�morisation du nombre d'�tiquettes */
      region32.ne = ne;
}
