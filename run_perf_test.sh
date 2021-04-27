#!/bin/bash

TEST_RESULT=perf_test_result.csv

REGISTRY_CMD="build/psf_core/psf_registry"
PUBLISHER_CMD="sleep 0.1; build/psf_perf_tests/publisher"
SUBSCRIBER_CMD="sleep 0.1; build/psf_perf_tests/subscriber > ${TEST_RESULT}"

kill_session()
{
	tmux kill-session -t psf_perf_tests
}

tmux new-session -d -s psf_perf_tests
trap kill_session EXIT
tmux new-window -t psf_perf_tests:1
tmux new-window -t psf_perf_tests:2
tmux send-keys -t psf_perf_tests:0 "${REGISTRY_CMD}" C-m
tmux send-keys -t psf_perf_tests:1 "${SUBSCRIBER_CMD}" C-m
tmux send-keys -t psf_perf_tests:2 "${PUBLISHER_CMD}" C-m
sleep 120
psf_perf_tests/bin/plot.py ${TEST_RESULT}
