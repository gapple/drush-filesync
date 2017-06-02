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

## Installation

### Global
#### Using Composer
1. Require `composer/installers`
    ```bash
    composer global require composer/installers
    ```

2. Configure the installer path for drush plugins in `~/.composer/composer.json`
    ```json
    {
      "extra": {
        "installer-paths": {
          "../.drush/plugins/{$name}": ["type:drupal-drush"]
        }
      }
    }
    ```

3. Require `gapple/drush-filesync`
    ```bash
    composer global require gapple/drush-filesync
    ```

#### Manual Installation
1. [Download the release package](https://github.com/gapple/drush-filesync/releases)
2. Unzip the package to `~/.drush/`


### Project
#### Using Composer
1. Require `composer/installers` in your project
    ```bash
    composer require composer/installers
    ```

2. Configure the installer path for drush plugins in your project's `composer.json`
    ```json
    {
      "extra": {
        "installer-paths": {
          "drush/contrib/{$name}": ["type:drupal-drush"]
        }
      }
    }
    ```

3. Require `gapple/drush-filesync` in your project
    ```bash
    composer require gapple/drush-filesync
    ```

#### Manual Installation
1. [Download the release package](https://github.com/gapple/drush-filesync/releases)
2. Unzip the package to the `drush` folder within your project
