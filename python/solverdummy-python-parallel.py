#!/usr/bin/env python3
#from __future__ import division

import argparse
import numpy as np

from mpi4py import MPI

import precice

parser = argparse.ArgumentParser()
parser.add_argument("configurationFileName",
                    help="Name of the xml config file.", type=str)
parser.add_argument("participantName", help="Name of the solver.", type=str)

try:
    args = parser.parse_args()
except SystemExit:
    print("")
    print("Usage: python ./solverdummy precice-config participant-name mesh-name")
    quit()

configuration_file_name = args.configurationFileName
participant_name = args.participantName

if participant_name == 'SolverOne':
    write_data_name = 'dataOne'
    read_data_name = 'dataTwo'
    mesh_name = 'MeshOne'

if participant_name == 'SolverTwo':
    read_data_name = 'dataOne'
    write_data_name = 'dataTwo'
    mesh_name = 'MeshTwo'

num_vertices = 1  # Number of vertices

comm = MPI.COMM_WORLD
solver_process_index = comm.Get_rank()
solver_process_size = comm.Get_size()

interface = precice.Interface(participant_name, configuration_file_name,
                              solver_process_index, solver_process_size)

mesh_id = interface.get_mesh_id(mesh_name)
dimensions = interface.get_dimensions()

vertices = np.zeros((num_vertices, dimensions))
read_data = np.zeros((num_vertices, dimensions))
write_data = np.zeros((num_vertices, dimensions))

for x in range(num_vertices):
    for y in range(0, dimensions):
        vertices[x, y] = x+solver_process_index
        read_data[x, y] = x+solver_process_index
        write_data[x, y] = x+solver_process_index

vertex_ids = interface.set_mesh_vertices(mesh_id, vertices)
read_data_id = interface.get_data_id(read_data_name, mesh_id)
write_data_id = interface.get_data_id(write_data_name, mesh_id)

dt = interface.initialize()

while interface.is_coupling_ongoing():
    if interface.is_action_required(
            precice.action_write_iteration_checkpoint()):
        print("DUMMY ({}): Writing iteration checkpoint".format(solver_process_index))
        interface.mark_action_fulfilled(
            precice.action_write_iteration_checkpoint())

    if interface.is_read_data_available():
        read_data = interface.read_block_vector_data(read_data_id, vertex_ids)

    write_data = read_data + 1

    if interface.is_write_data_required(dt):
        interface.write_block_vector_data(
            write_data_id, vertex_ids, write_data)

    print("DUMMY ({}): Advancing in time".format(solver_process_index))
    dt = interface.advance(dt)

    if interface.is_action_required(
            precice.action_read_iteration_checkpoint()):
        print("DUMMY ({}): Reading iteration checkpoint".format(solver_process_index))
        interface.mark_action_fulfilled(
            precice.action_read_iteration_checkpoint())

interface.finalize()
print("DUMMY ({}): Closing python solver dummy...".format(solver_process_index))