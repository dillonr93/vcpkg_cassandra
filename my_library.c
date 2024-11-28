#include "cassandra.h"

void method(){
    CassCluster* cluster = cass_cluster_new();

    cass_cluster_free(cluster);
}