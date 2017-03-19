# contrib/

This folder contains contributions that may be useful to users of this library, but
have a too specialized audience to become part of the main tree.

In particular, things in here will have a higher rate of bit-rot, since
contributors are not required to and may be unable to check whether their
changes break any of them.

## Windows support

### Configuring and Building Samples

```cmd
> cd contrib
> WinBuild.cmd
```

The `WinBuild.cmd` will set up temporal ENV variables and build binaries in (matplotlib root)/examples with the Release configuration.
