@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Create a Patch</div>

                    <div class="panel-body">
                        <form role="form" method="POST" action="{{action('PatchController@create_store')}}">
                            {{ csrf_field() }}

                            @if ($errors->any())
                                <div class="alert alert-danger">
                                    <ul>
                                        @foreach ($errors->all() as $error)
                                            <li>{{ $error }}</li>
                                        @endforeach
                                    </ul>
                                </div>
                            @endif
                            <input type="hidden" id="id_bug" name="id_bug" value="">

                            <strong>Select bugs, that patch fixes.</strong>
                            <hr>
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
                                            <tbody id="select_tr">
                                            @foreach($bugs as $bug)
                                                <tr id="{{$bug->id_bug}}" onclick="reply_click(this)">
                                                    <td class="col-md-1"> {{ $bug->id_bug }}  </td>
                                                    <td class="col-md-1"> {{ $bug->typ }} </td>
                                                    <td class="col-md-1"> {{ $bug->zavaznost }} </td>
                                                    <td class="col-md-1"> {{ $bug->meno }} </td>
                                                    <td class="col-md-1"> {{ $bug->id_patch }} </td>
                                                    <td> {{ $bug->popis_bugu }} </td>
                                                </tr>
                                            @endforeach
                                            </tbody>
                                        </table>
                                    </div>
                                </div>
                            </div>

                            <hr>
                            <div class="form-group">
                                <label>Diff of code</label>
                                <textarea name="diff" class="form-control" rows="15" id="diff" placeholder="Please insert diff of your code"></textarea>
                            </div>
                            <div id="buttons">
                                <button id="submit" name="user" class="btn btn-default" value="{{Auth::user()->name}}" onclick="convert()">Submit patch</button>
                                <button type="reset" class="btn btn-default">Reset fiels</button>
                            </div>
                        </form>
                    </div>
                </div>
            </div>
        </div>
    </div>


    <script src="{{ asset('js/script.js') }}"></script>

    <!-- jQuery -->
    <script src="{{ asset('js/jquery.js') }}"></script>

    <!-- Page level plugin JavaScript-->
    <script src="{{ asset('js/datatables/jquery.dataTables.js') }}"></script>
    <script src="{{ asset('js/datatables/dataTables.bootstrap4.js') }}"></script>

    <!-- Custom scripts for this page-->
    <script src="{{ asset('js/datatables.js') }}"></script>
@endsection
