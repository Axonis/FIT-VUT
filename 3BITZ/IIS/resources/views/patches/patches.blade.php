@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Patches</div>

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
                                            <th>ID of Patch</th>
                                            <th>Approved</th>
                                            <th>Written by</th>
                                            <th>Release day</th>
                                            <th>Approval day</th>

                                        </tr>
                                        </thead>
                                        <tfoot>
                                        <tr>
                                            <th>ID of Patch</th>
                                            <th>Approved</th>
                                            <th>Written by</th>
                                            <th>Release day</th>
                                            <th>Approval day</th>

                                        </tr>
                                        </tfoot>
                                        <tbody>
                                        @foreach($patches as $item)
                                            <tr>
                                                <td><a href="/patches/{{$item->id_patch}}"> {{ $item->id_patch}} </a>
                                                </td>
                                                @if($item->schvalenie)
                                                    <td>True</td>
                                                @else
                                                    <td>False</td>
                                                @endif
                                                <td><a href="/profile/{{ $item->name_vydany }}">{{ $item->name_vydany }} </a></td>
                                                <td>{{ $item->datum_vydania }}</td>
                                                <td>{{ $item->datum_zavedenia }}</td>
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


