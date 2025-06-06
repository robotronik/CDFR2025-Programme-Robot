void a_star(int start_x, int start_y, int goal_x, int goal_y) {
    for (int x = 0; x < HEIGHT; x++) {
        for (int y = 0; y < WIDTH; y++) {
            nodes[x][y].x = x;
            nodes[x][y].y = y;
            nodes[x][y].g = INT_MAX;
            nodes[x][y].f = INT_MAX;
            nodes[x][y].visited = false;
        }
    }

    nodes[start_x][start_y].g = 0;
    nodes[start_x][start_y].h = heuristic(start_x, start_y, goal_x, goal_y);
    nodes[start_x][start_y].f = nodes[start_x][start_y].h;

    position_t open[MAX_OPEN_SIZE];
    int open_size = 0;
    open[open_size++] = (position_t){start_x, start_y};

    while (open_size > 0) {
        // Trouver le nœud avec le plus petit f
        int best_idx = 0;
        for (int i = 1; i < open_size; i++) {
            position_t p = open[i];
            if (nodes[p.x][p.y].f < nodes[open[best_idx].x][open[best_idx].y].f) {
                best_idx = i;
            }
        }

        position_t current = open[best_idx];
        // Retirer le nœud de open
        open[best_idx] = open[--open_size];

        int cx = current.x, cy = current.y;

        // Objectif atteint
        if (cx == goal_x && cy == goal_y) {
            return;
        }

        nodes[cx][cy].visited = true;

        int directions[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
        for (int d = 0; d < 4; d++) {
            int nx = cx + directions[d][0];
            int ny = cy + directions[d][1];

            if (nx < 0 || ny < 0 || nx >= HEIGHT || ny >= WIDTH)
                continue;

            if (costmap[nx][ny] >= OBSTACLE_COST)
                continue;

            // NOTE IMPORTANTE : on n'ignore plus les nœuds visités ici

            int tentative_g = nodes[cx][cy].g + 1 + costmap[nx][ny] ;

            if (tentative_g < nodes[nx][ny].g) {
                nodes[nx][ny].g = tentative_g;
                nodes[nx][ny].h = heuristic(nx, ny, goal_x, goal_y);
                nodes[nx][ny].f = nodes[nx][ny].g + nodes[nx][ny].h;
                nodes[nx][ny].parent_x = cx;
                nodes[nx][ny].parent_y = cy;

                // Ajouter à open si pas déjà dedans
                bool in_open = false;
                for (int i = 0; i < open_size; i++) {
                    if (open[i].x == nx && open[i].y == ny) {
                        in_open = true;
                        break;
                    }
                }
                if (!in_open) {
                    if (open_size >= MAX_OPEN_SIZE) {
                        printf("Erreur : dépassement de la taille de open (MAX_OPEN_SIZE=%d)\n", MAX_OPEN_SIZE);
                        return;
                    }
                    open[open_size++] = (position_t){nx, ny};
                }
            }
        }
    }

    printf("Aucun chemin trouvé.\n");
}
