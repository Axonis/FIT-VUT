@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">All tickets</div>

                    <div class="panel-body">
                        @if (session('status'))
                            <div class="alert alert-success">
                                {{ session('status') }}
                            </div>
                        @endif

                        <div class="card mb-3">
                            <div class="card-body">
                                <div class="table-responsive">
                                    <table class="table table-bordered" id="dataTable" width="100%" cellspacing="0">
                                        <thead>
                                        <tr>
                                            <th>ID of ticket</th>
                                            <th>State</th>
                                            <th>Assigned to</th>
                                            <th>Created by</th>
                                            <th>Description</th>
                                        </tr>
                                        </thead>
                                        <tfoot>
                                        <tr>
                                            <th>ID of ticket</th>
                                            <th>State</th>
                                            <th>Assigned to</th>
                                            <th>Created by</th>
                                            <th>Description</th>
                                        </tr>
                                        </tfoot>
                                        <tbody>
                                        @foreach($tickets as $ticket)
                                            <tr>
                                                <td><a href="tickets/{{$ticket->id_ticket}}"> {{ $ticket->id_ticket }} </a></td>
                                                <td> {{ $ticket->stav }} </td>
                                                <td> <a href="/profile/{{ $ticket->name_prideleny }}">{{ $ticket->name_prideleny }}</a></td>
                                                <td> <a href="/profile/{{ $ticket->name_vytvoreny }}">{{ $ticket->name_vytvoreny }} </a> </td>
                                                <td> {{ $ticket->popis }}</td>
                                            </tr>
                                        @endforeach
                                        </tbody>
                                    </table>
                                </div>
                            </div>
                        </div>



                    </div>
                </div>
            </div>
        </div>
    </div>


    <!-- jQuery -->
    <script src="{{ asset('js/jquery.js') }}"></script>

    <!-- Page level plugin JavaScript-->
    <script src="{{ asset('js/datatables/jquery.dataTables.js') }}"></script>
    <script src="{{ asset('js/datatables/dataTables.bootstrap4.js') }}"></script>

    <!-- Custom scripts for this page-->
    <script src="{{ asset('js/datatables.js') }}"></script>

@endsection