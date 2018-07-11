@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Create a Module</div>

                    <div class="panel-body">
                        <form role="form" method="POST" action="{{action('ModuleController@create_store')}}">
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


                            <div class="form-group">
                                <label>Modul name</label>
                                <input name="name" class="form-control" id="diff" placeholder="Please write modul name">
                            </div>


                            <div class="form-group">
                                <label>Choose language in which module is written</label>
                                <select class="form-control" name="lang">
                                    @foreach($langs as $lang)
                                        <option>{{$lang->name}}</option>
                                    @endforeach
                                </select>
                            </div>

                            <div class="form-group">
                                <label>Choose responsible programmer</label>
                                <select class="form-control" name="user">
                                    @foreach($users as $user)
                                        <option>{{$user->name}}</option>
                                    @endforeach
                                </select>
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

    <!-- jQuery -->
    <script src="{{ asset('js/jquery.js') }}"></script>

    <!-- Page level plugin JavaScript-->
    <script src="{{ asset('js/datatables/jquery.dataTables.js') }}"></script>
    <script src="{{ asset('js/datatables/dataTables.bootstrap4.js') }}"></script>

    <!-- Custom scripts for this page-->
    <script src="{{ asset('js/datatables.js') }}"></script>
@endsection
