for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                for(int k=0; k<4; k++){
                    init_originalcoo();
                    isblock0 = canexist0;
                    isblock1 = canexist1;
                    vector<int> modevec2 = {i, j, k};
                    for(auto mode:modevec2) lotation.lotate90(mode, isblock1, originalcoo);
                    build_block();
                }
            }
        }