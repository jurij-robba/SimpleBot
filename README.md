# Simplebot

An application that parses simulated orderbook and provides liquidity to simulated market.

## Usage

Executable takes no input parameters. After it is built it can be run using

```
./Simplebot
```

## Building

`make` builds application.
`make run` build and runs application
`make clean` cleans the directory

## Testing

Tests are written using [Catch2](https://github.com/catchorg/Catch2) framework.

To download needed header / update to latest version run

```
make update_test
```

Once downloaded you can use `make test` to run the tests
To clean tests run `make clean_test`


