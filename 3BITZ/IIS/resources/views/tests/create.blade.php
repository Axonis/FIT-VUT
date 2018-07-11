@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Create a Test</div>

                    <div class="panel-body">
                        <form role="form" method="POST" action="{{action('TestController@create_store')}}">
                            {{ csrf_field() }}


                            <input type="hidden" id="id_patch" name="id_patch" value="">

                            <div class="card mb-3">
                                <div class="card-body">
                                    <div class="table-responsive">
                                        <table class="table table-bordered" id="dataTable" width="100%" cellspacing="0">
                                            <thead>
                                            <tr>
                                                <th>ID of Patch</th>
                                                <th>Written by</th>
                                                <th>Release day</th>
                                                <th>Fixed bugs</th>

                                            </tr>
                                            </thead>
                                            <tfoot>
                                            <tr>
                                                <th>ID of Patch</th>
                                                <th>Written by</th>
                                                <th>Release day</th>
                                                <th>Fixed bugs</th>

                                            </tr>
                                            </tfoot>
                                            <tbody id="select_tr">
                                            @foreach($data as $item)
                                                <tr id="{{ $item[0]->id_patch}}" onclick="single_click(this)">
                                                    <td class="col-md-2"> {{ $item[0]->id_patch}} </td>
                                                    <td class="col-md-2"> {{ $item[0]->name_vydany }} </td>
                                                    <td class="col-md-2"> {{ $item[0]->datum_vydania }} </td>
                                                    <td> {{ $item[1] }} </td>
                                                </tr>
                                            @endforeach
                                            </tbody>
                                        </table>
                                    </div>
                                </div>
                            </div>

                            <div class="form-group">
                                <label>Test code</label>
                                <textarea name="code" class="form-control" rows="15" id="diff" placeholder="Please insert diff of your code"></textarea>
                            </div>
                            <div id="buttons">
                                <button name="user" class="btn btn-default" value="{{Auth::user()->name}}">Submit Button</button>
                                <button type="reset" class="btn btn-default">Reset Button</button>
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
