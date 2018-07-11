@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Submit a ticket</div>


                    <div class="panel-body">
                        <form role="form" method="POST" action="{{ action('TicketController@store') }}">
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

                            <input type="hidden" name="name_vytvoreny" value="{{ Auth::user()->name }}">
                            <input type="hidden" id="id_bug" name="id_bug" value="">


                            <div class="form-group">
                                <label for="popis_ticketu">Description of ticket</label>
                                <textarea name="popis_ticketu" class="form-control" rows="3" id="popis_ticketu"
                                          placeholder="Preferably what is the issue, how does it affect you and your work">{{ (old('popis_ticketu')) ? old('popis_ticketu') : '' }}
                                </textarea>
                            </div>

                            <div class="panel-group" id="accordion">
                                <div class="panel panel-default">
                                    <div class="panel-heading">
                                        <h4 class="panel-title">
                                            <a data-toggle="collapse" data-parent="#accordion" href="#collapse1">
                                                Specify a new bug</a>
                                        </h4>
                                    </div>
                                    <div id="collapse1" class="panel-collapse collapse @if (old('meno_modulu')) in @endif">
                                        <div class="panel-body">
                                            <div class="form-group">
                                                <label for="popis_bugu">Description of a bug</label>
                                                <textarea name="popis_bugu" class="form-control" rows="3" id="popis_bugu"
                                                          placeholder="Preferably what causes the bug, what is the root issue">{{ (old('popis_bugu')) ? old('popis_bugu') : '' }}
                                                </textarea>

                                            </div>

                                            <div class="form-group">
                                                <label>Severity of a Bug</label>
                                                @foreach($severity as $item)
                                                    <div class="radio">
                                                        <label>
                                                            <input type="radio" name="zavaznost" id="{{$item}}" value="{{$item}}"
                                                                   @if (old('zavaznost') == $item) checked @endif>
                                                            {{$item}}
                                                        </label>
                                                    </div>
                                                @endforeach
                                            </div>

                                            <div class="form-group">
                                                <label>Bug is assigned to module : </label>
                                                <select class="form-control" name="meno_modulu">
                                                    @foreach($modules as $module)
                                                        <option @if (old('meno_modulu') == $module->meno) selected @endif>{{$module->meno}}</option>
                                                    @endforeach
                                                </select>
                                            </div>

                                            <div class="form-group">
                                                <label>Type of bug</label>
                                                <select class="form-control" name="typ">
                                                    @foreach($bug_types as $type)
                                                        <option @if (old('typ') == $type) selected @endif>{{$type}}</option>
                                                    @endforeach
                                                </select>
                                            </div>

                                            <div class="form-group">
                                                <label>Rate of vulnerability</label> (Fill out only if bug is a vulnerability too.)
                                                @foreach($vulner as $item)
                                                    <div class="radio">
                                                        <label>
                                                            <input type="radio" name="miera_nebezpecenstva" id="{{$item}}" value="{{$item}}"
                                                                   @if (old('miera_nebezpecenstva') == $item) checked @endif
                                                            >
                                                            {{$item}}
                                                        </label>
                                                    </div>
                                                @endforeach
                                            </div>
                                        </div>
                                    </div>
                                </div>
                                <div class="panel panel-default">
                                    <div class="panel-heading">
                                        <h4 class="panel-title">
                                            <a data-toggle="collapse" data-parent="#accordion" href="#collapse2">
                                                Search for existing bug</a>
                                        </h4>
                                    </div>
                                    <div id="collapse2" class="panel-collapse collapse">
                                        <div class="panel-body">
                                            Click on bug you would like to report:
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
                                        </div>
                                    </div>
                                </div>
                            </div>



                            <div id="buttons">
                                <button id="submit" type="submit" class="btn btn-default" onclick="convert()">Submit Ticket</button>
                                <button type="reset" class="btn btn-default">Reset fields</button>
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
