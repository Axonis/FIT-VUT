@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">All bugs</div>

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
                                            <th>Bug</th>
                                            <th>Type</th>
                                            <th>Severity</th>
                                            <th>Module</th>
                                            <th>Patch</th>
                                            <th>Description</th>
                                        </tr>
                                        </thead>
                                        <tfoot>
                                        <tr>
                                            <th>Bug</th>
                                            <th>Type</th>
                                            <th>Severity</th>
                                            <th>Module</th>
                                            <th>Patch</th>
                                            <th>Description</th>
                                        </tr>
                                        </tfoot>
                                        <tbody>
                                        @foreach($bugs as $bug)
                                            <tr>
                                                <td class="col-md-1"><a href="/bugs/{{$bug->id_bug}}"> {{ $bug->id_bug }} </a></td>
                                                <td class="col-md-1"> {{ $bug->typ }} </td>
                                                <td class="col-md-1"> {{ $bug->zavaznost }} </td>
                                                <td class="col-md-1"> {{ $bug->meno }} </td>
                                                <td class="col-md-1"> <a href="/patches/{{$bug->id_patch}}">{{$bug->id_patch}}</a></td>
                                                <td> {{ $bug->popis_bugu }} </td>
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