Drush File-sync
===============

Drush command to sync files directory between environments

## Usage

```
Arguments:
 source                                             Source alias
 destination                                        Destination alias

Options:
 --only-private                                     Only sync the private files directory
 --only-public                                      Only sync the public files directory
 
Examples:
 drush fsync @source @destination                   Sync both the public and private files directories
 drush fsync --only-public @source @destination     Only sync the public files directory
 
Aliases: fsync
```
