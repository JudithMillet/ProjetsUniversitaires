def compute_one_cluster(object img, object visited, int x, int y, int min_size = 2000, int max_size = 8000):
	# calcul le cluster du pixel (x, y)
	cdef int height, width, channel
	height, width, channel = img.shape

	cluster = []
	pile = [(x, y)]
	while(len(pile) != 0):
		x, y = pile.pop()

		if(visited[y][x]):
			continue

		cluster.append((x, y))
		visited[y][x] = True

		if(x-1 >= 0 and not visited[y][x-1]):
			pile.append((x-1, y))
		if(x+1 < width and not visited[y][x+1]):
			pile.append((x+1, y))
		if(y-1 >= 0 and not visited[y-1][x]):
			pile.append((x, y-1))
		if(y+1 < height and not visited[y+1][x]):
			pile.append((x, y+1))

	if (len(cluster) < min_size) or (len(cluster) > max_size):
		return None
	return cluster


def compute_clusters(object edges, object img, int min_size = 2000, int max_size = 8000):
	# calcule tout les clusters
	cdef int height, width, channel
	height, width, channel = img.shape
	cdef object clusters = []

	cdef object visited = (edges !=0) #edges is a numpy array, so the results of edges !=0 is a numpy array of boolean

	for x in range(width):
		for y in range(height):
			if not visited[y][x]:
				cluster = compute_one_cluster(img, visited, x, y, min_size, max_size)
				if(cluster != None):
					clusters.append(cluster)

	return clusters