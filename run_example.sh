#!/bin/bash

TEST_RESULT=perf_test_result.csv

REGISTRY_CMD="build/psf_core/psf_registry"
PING_SERVER_CMD="sleep 0.1; build/psf_examples/ping_server"
PING_CLIENT_CMD="sleep 0.1; build/psf_examples/ping_client"

SESSION=psf_examples

kill_session()
{
	tmux kill-session -t ${SESSION}
}

tmux new-session -d -s ${SESSION}
trap kill_session EXIT
tmux split-window -v
tmux split-window -v
tmux select-pane -t 0; tmux send-keys "$REGISTRY_CMD" C-m
tmux select-pane -t 1; tmux send-keys "$PING_SERVER_CMD" C-m
tmux select-pane -t 2; tmux send-keys "$PING_CLIENT_CMD" C-m
tmux select-window -t ${SESSION}:1
tmux attach-session -t ${SESSION}
