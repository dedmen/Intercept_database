# what

use MariaDB / MySQL in Arma

# how

define MariaDB or MySQL connection parameters by creating `config.yaml` in the mod directory:

```yaml
accounts: 
  foo:
    ip: 127.0.0.1
    username: us3r
    password: s3cr3t
    database: foo 
```

```sqf
private _connection = dbCreateConnection "foo";
private _query = dbPrepareQuery "select 1";
private _result = _connection dbExecute _query;
systemChat format ["xxx %1", _result];
```
