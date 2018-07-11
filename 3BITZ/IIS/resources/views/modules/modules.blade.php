@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">All modules</div>

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
                                            <th>Module ID</th>
                                            <th>Name</th>
                                            <th>Error rate</th>
                                            <th>Responsible person</th>
                                            <th>Language</th>
                                        </tr>
                                        </thead>
                                        <tfoot>
                                        <tr>
                                            <th>Module ID</th>
                                            <th>Name</th>
                                            <th>Error rate</th>
                                            <th>Responsible person</th>
                                            <th>Language</th>
                                        </tr>
                                        </tfoot>

                                        <tbody>
                                        @foreach($modules as $module)
                                            <tr>
                                                <td class="col-md-1"><a href="/modules/{{$module->id_modul}}"> {{ $module->id_modul }} </a></td>
                                                <td class="col-md-1"> {{ $module->meno }} </td>
                                                <td class="col-md-1"> {{ $module->chybovost }}% </td>
                                                <td class="col-md-1"><a href="/profile/{{ $module->name_zodpovedny }}">{{ $module->name_zodpovedny }}</a> </td>
                                                <td class="col-md-1"> {{ $module->name}} </td>
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