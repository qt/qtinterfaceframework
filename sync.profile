%modules = ( # path to module name map
    "QtInterfaceFramework" => "$basedir/src/interfaceframework",
    "QtIfVehicleFunctions" => "$basedir/src/ifvehiclefunctions;$out_basedir/src/ifvehiclefunctions",
    "QtIfMedia" => "$basedir/src/ifmedia",
    "QtIfRemoteObjectsHelper" => "$basedir/src/helper/remoteobjects",
);
%moduleheaders = ( # restrict the module headers to those found in relative path
);
%classnames = (
     "qifqueryterm.h" => "QIfAbstractQueryTerm", "QIfConjunctionTerm", "QIfScopeTerm", "QIfFilterTerm", "QIfOrderTerm",
     "qifstandarditem.h" => "QIfFilterAndBrowseModelItem"
);
%deprecatedheaders = (
);
# Module dependencies.
# Every module that is required to build this module should have one entry.
# Each of the module version specifiers can take one of the following values:
#   - A specific Git revision.
#   - any git symbolic ref resolvable from the module's repository (e.g. "refs/heads/master" to track master branch)
#
%dependencies = (
    "qtbase" => "",
    "qtdeclarative" => "",
    "qtmultimedia" => ""
);

%inject_headers = (
    "$basedir/src/helper/remoteobjects" => [ "rep_qifpagingmodel_source.h", "rep_qifpagingmodel_replica.h" ]
)

