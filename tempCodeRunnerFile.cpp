
            ismonomino[i].erase(all(ismonomino[i]));
            ismonomino[i].shrink_to_fit();
        }
        ismonomino.erase(all(ismonomino));
        ismonomino.shrink_to_fit();