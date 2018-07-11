@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Dashboard</div>

                    <div class="panel-body">
                        <div class="col-md-12">

                            @if ($num_of_tickets)
                                <hr>
                                <h4>There is {{$num_of_tickets}} new {{$count_all}} in system.</h4>
                                <hr>

                                <div class="table-responsive">
                                    <table class="table table-bordered" id="dataTable" width="100%" cellspacing="0">
                                        <thead>
                                        <tr>
                                            <th>ID of Ticket</th>
                                            <th>Description</th>

                                        </tr>
                                        </thead>
                                        <tbody>
                                        @foreach($tickets as $item)
                                            <tr>
                                                <td class="col-md-2"><a
                                                            href="/tickets/{{$item->id_ticket}}"> {{ $item->id_ticket}} </a>
                                                </td>
                                                <td>{{ $item->popis }}</td>
                                            </tr>
                                        @endforeach
                                        </tbody>
                                    </table>
                                </div>

                            @else
                                <h4>There is no new ticket.</h4>
                            @endif
                        </div>

                        @if(Auth::user()->group == 'Programmer' or Auth::user()->group == 'Admin')
                            <div class="col-md-6">
                                <hr>
                                <h4>You have {{$num_of_my_tickets}} assigned {{$count_assigned}}.</h4>
                                <hr>
                                <div class="table-responsive">
                                    <table class="table table-bordered" id="dataTable" width="100%" cellspacing="0">
                                        <thead>
                                        <tr>
                                            <th>ID of Ticket</th>
                                            <th>Description</th>

                                        </tr>
                                        </thead>
                                        <tbody>
                                        @foreach($my_tickets as $item)
                                            <tr>
                                                <td class="col-md-2"><a
                                                            href="/tickets/{{$item->id_ticket}}"> {{ $item->id_ticket}} </a>
                                                </td>
                                                <td>{{ $item->popis }}</td>
                                            </tr>
                                        @endforeach
                                        </tbody>
                                    </table>
                                </div>
                            </div>

                            <div class="col-md-6">
                                <hr>
                                <h4>You have {{sizeof($bugs_on_my_module)}} new {{$count_module}} on your modules.</h4>
                                <hr>
                                <div class="table-responsive">
                                    <table class="table table-bordered" id="dataTable" width="100%" cellspacing="0">
                                        <thead>
                                        <tr>
                                            <th>ID of Ticket</th>
                                            <th>Description</th>

                                        </tr>
                                        </thead>
                                        <tbody>
                                        @foreach($bugs_on_my_module as $item)
                                            <tr>
                                                <td class="col-md-2"><a
                                                            href="/tickets/{{$item->id_ticket}}"> {{ $item->id_ticket}} </a>
                                                </td>
                                                <td>{{ $item->popis }}</td>
                                            </tr>
                                        @endforeach
                                        </tbody>
                                    </table>
                                </div>
                            </div>

                        @endif
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
