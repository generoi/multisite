Multisite Aggregate
===================

Installation
------------

Configure which entities/bundles should be aggregated in a shared settings.php file

// Entities and bundles to aggregate on the main site.
$conf['multisite_aggregate_types'] = array(
  'node' => array('blog'),
);

