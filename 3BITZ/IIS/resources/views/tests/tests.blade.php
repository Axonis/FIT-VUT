@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">All tests</div>

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
                                            <th>Test</th>
                                            <th>Start date</th>
                                            <th>End date</th>
                                            <th>Result</th>
                                            <th>Tested patch</th>
                                            <th>Approved by</th>
                                            <th>Written by</th>
                                        </tr>
                                        </thead>
                                        <tfoot>
                                        <tr>
                                            <th>Test</th>
                                            <th>Start date</th>
                                            <th>End date</th>
                                            <th>Result</th>
                                            <th>Tested patch</th>
                                            <th>Approved by</th>
                                            <th>Written by</th>
                                        </tr>
                                        </tfoot>
                                        <tbody>
                                        @foreach($tests as $test)
                                            <tr>
                                                <td class="col-md-1"><a href="/tests/{{$test->id}}"> {{ $test->id }} </a></td>
                                                <td class="col-md-1"> {{ $test->datum_zaciatku }} </td>
                                                <td class="col-md-1"> {{ $test->datum_konca }} </td>
                                                <td class="col-md-1"> {{ $test->hodnotenie }} </td>
                                                <td class="col-md-1"> <a href="/patches/{{$test->id_patch}}">{{$test->id_patch}}</a></td>
                                                <td> <a href="/profile/{{$test->name_schvaleny}}">{{ $test->name_schvaleny }} </a></td>
                                                <td> {{ $test->napisal }} </td>
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