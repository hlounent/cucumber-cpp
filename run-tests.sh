#!/bin/sh

BUILD_DIR=$1
RUNNER=$2
RUNNER_ARGS=$3

$RUNNER $RUNNER_ARGS ./$BUILD_DIR/examples/FeatureShowcase/FeatureShowcaseSteps > /dev/null &
sleep 1; cucumber examples/FeatureShowcase
$RUNNER $RUNNER_ARGS ./$BUILD_DIR/examples/CalcQt/BoostCalculatorQtSteps > /dev/null &
sleep 1; cucumber examples/CalcQt
$RUNNER $RUNNER_ARGS ./$BUILD_DIR/examples/Calc/GTestCalculatorSteps > /dev/null &
sleep 1; cucumber examples/Calc
$RUNNER $RUNNER_ARGS ./$BUILD_DIR/examples/Calc/BoostCalculatorSteps > /dev/null &
sleep 1; cucumber examples/Calc
